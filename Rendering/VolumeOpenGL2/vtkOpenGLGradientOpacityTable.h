/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkOpenGLGradientOpacityTable.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkOpenGLGradientOpacityTable_h_
#define __vtkOpenGLGradientOpacityTable_h_

#include <vtkPiecewiseFunction.h>
#include <vtkTextureObject.h>
#include <vtkVolumeMapper.h>

#include <vtk_glew.h>

//----------------------------------------------------------------------------
class vtkOpenGLGradientOpacityTable
{
public:
  //--------------------------------------------------------------------------
  vtkOpenGLGradientOpacityTable(int width = 1024)
    {
      this->TextureObject = 0;
      this->TextureWidth = width;
      this->LastSampleDistance = 1.0;
      this->Table = 0;
      this->LastInterpolation = -1;
      this->LastRange[0] = this->LastRange[1] = 0.0;
    }

  //--------------------------------------------------------------------------
  ~vtkOpenGLGradientOpacityTable()
    {
      if (this->TextureObject != 0)
        {
        this->TextureObject->Delete();
        this->TextureObject = 0;
        }

      if (this->Table != 0)
        {
        delete[] this->Table;
        this->Table=0;
        }
    }

  // Bind texture.
  //--------------------------------------------------------------------------
  void Bind()
    {
    if (!this->TextureObject)
      {
      return;
      }
    this->TextureObject->Activate();
    }

  // Update opacity tranfer function texture.
  //--------------------------------------------------------------------------
  void Update(vtkPiecewiseFunction* gradientOpacity,
              double sampleDistance,
              double range[2],
              double vtkNotUsed(unitDistance),
              int filterValue,
              vtkOpenGLRenderWindow* renWin)
    {
    bool needUpdate=false;

    if (!this->TextureObject)
      {
      this->TextureObject = vtkTextureObject::New();
      }

    this->TextureObject->SetContext(renWin);

    if (this->LastRange[0] != range[0] ||
        this->LastRange[1] != range[1])
      {
      this->LastRange[0] = range[0];
      this->LastRange[1] = range[1];
      needUpdate = true;
      }

    if(gradientOpacity->GetMTime() > this->BuildTime ||
       this->TextureObject->GetMTime() > this->BuildTime ||
       this->LastSampleDistance != sampleDistance ||
       needUpdate || !this->TextureObject->GetHandle())
      {
      if(this->Table == 0)
        {
        this->Table = new float[this->TextureWidth];
        }

      gradientOpacity->GetTable(0,
                              (this->LastRange[1] - this->LastRange[0]) * 0.25,
                              this->TextureWidth, this->Table);

      this->TextureObject->CreateAlphaFromRaw(this->TextureWidth,
                                              vtkTextureObject::alpha16,
                                              VTK_FLOAT,
                                              this->Table);

      this->TextureObject->Activate();
      this->TextureObject->SetWrapS(vtkTextureObject::ClampToEdge);
      this->TextureObject->SetMagnificationFilter(filterValue);
      this->TextureObject->SetMinificationFilter(filterValue);
      this->BuildTime.Modified();
      }

    if(this->LastInterpolation != filterValue)
      {
      this->LastInterpolation = filterValue;
      this->TextureObject->SetMagnificationFilter(filterValue);
      this->TextureObject->SetMinificationFilter(filterValue);
      }
    }

  // Get the texture unit
  //--------------------------------------------------------------------------
  int GetTextureUnit(void)
    {
    if (!this->TextureObject)
      {
      return -1;
      }
    return this->TextureObject->GetTextureUnit();
    }

  //--------------------------------------------------------------------------
  void ReleaseGraphicsResources(vtkWindow *window)
    {
    if (this->TextureObject)
      {
      this->TextureObject->ReleaseGraphicsResources(window);
      this->TextureObject->Delete();
      this->TextureObject = 0;
      }
    }

protected:
//  GLuint TextureId;
  vtkTextureObject* TextureObject;
  int TextureWidth;

  double LastSampleDistance;
  vtkTimeStamp BuildTime;
  float* Table;
  int LastInterpolation;
  double LastRange[2];
private:
  vtkOpenGLGradientOpacityTable(const vtkOpenGLGradientOpacityTable&);
  vtkOpenGLGradientOpacityTable& operator=(const vtkOpenGLGradientOpacityTable&);
};

//-----------------------------------------------------------------------------
class vtkOpenGLGradientOpacityTables
{
public:
  //--------------------------------------------------------------------------
  vtkOpenGLGradientOpacityTables(unsigned int numberOfTables)
    {
    this->Tables = new vtkOpenGLGradientOpacityTable[numberOfTables];
    this->NumberOfTables = numberOfTables;
    }

  //--------------------------------------------------------------------------
  ~vtkOpenGLGradientOpacityTables()
    {
    delete [] this->Tables;
    }

  // Get opacity table at a given index.
  //--------------------------------------------------------------------------
  vtkOpenGLGradientOpacityTable* GetTable(unsigned int i)
    {
    if (i >= this->NumberOfTables)
      {
      return NULL;
      }
    return &this->Tables[i];
    }

  // Get number of tables.
  //--------------------------------------------------------------------------
  unsigned int GetNumberOfTables()
    {
    return this->NumberOfTables;
    }

  //--------------------------------------------------------------------------
  void ReleaseGraphicsResources(vtkWindow *window)
    {
    for (unsigned int i = 0; i < this->NumberOfTables; ++i)
      {
      this->Tables[i].ReleaseGraphicsResources(window);
      }
    }
private:
  unsigned int NumberOfTables;
  vtkOpenGLGradientOpacityTable* Tables;

  // vtkOpenGLGradientOpacityTables (Not implemented)
  vtkOpenGLGradientOpacityTables();

  // vtkOpenGLGradientOpacityTables (Not implemented)
  vtkOpenGLGradientOpacityTables(const vtkOpenGLGradientOpacityTables &other);

  // operator = (Not implemented)
  vtkOpenGLGradientOpacityTables &operator=(const vtkOpenGLGradientOpacityTables &other);
};

#endif // __vtkOpenGLGradientOpacityTable_h_
// VTK-HeaderTest-Exclude: vtkOpenGLGradientOpacityTable.h
