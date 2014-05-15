/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkExternalOpenGLRenderer.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkExternalOpenGLRenderer.h"

#include "vtkObjectFactory.h"
#include "vtkOpenGL.h"
#include "vtkOpenGLError.h"
#include "vtkRenderWindow.h"
#include "vtkTexture.h"

vtkStandardNewMacro(vtkExternalOpenGLRenderer);

//----------------------------------------------------------------------------
vtkExternalOpenGLRenderer::vtkExternalOpenGLRenderer()
{
  this->ClearBuffers = 0;
}

//----------------------------------------------------------------------------
vtkExternalOpenGLRenderer::~vtkExternalOpenGLRenderer()
{
}

//----------------------------------------------------------------------------
void vtkExternalOpenGLRenderer::Clear()
{
  vtkOpenGLClearErrorMacro();

  GLbitfield  clear_mask = 0;

  if (!this->Transparent() && !this->GetClearBuffers())
    {
    glClearColor( static_cast<GLclampf>(this->Background[0]),
                  static_cast<GLclampf>(this->Background[1]),
                  static_cast<GLclampf>(this->Background[2]),
                  static_cast<GLclampf>(0.0));
    clear_mask |= GL_COLOR_BUFFER_BIT;
    }

  if (!this->GetPreserveDepthBuffer())
    {
    glClearDepth(static_cast<GLclampf>(1.0));
    clear_mask |= GL_DEPTH_BUFFER_BIT;
    }

  vtkDebugMacro(<< "glClear\n");
  glClear(clear_mask);

  // If gradient background is turned on, draw it now.
  if (!this->Transparent() &&
      (this->GradientBackground || this->TexturedBackground))
    {
    double tile_viewport[4];
    this->GetRenderWindow()->GetTileViewport(tile_viewport);
    glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_1D);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glShadeModel(GL_SMOOTH); // color interpolation

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    {
      glLoadIdentity();
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      {
        glLoadIdentity();
        glOrtho(
          tile_viewport[0],
          tile_viewport[2],
          tile_viewport[1],
          tile_viewport[3],
          -1.0, 1.0);

        //top vertices
        if(this->TexturedBackground && this->BackgroundTexture)
          {
          glEnable(GL_TEXTURE_2D);

          this->BackgroundTexture->Render(this);

          // NOTE: By default the mode is GL_MODULATE. Since the user
          // cannot set the mode, the default is set to replace.
          glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

          // NOTE: vtkTexture Render enables the alpha test
          // so that no buffer is affected if alpha of incoming fragment is
          // below the threshold. Here we have to enable it so that it won't
          // rejects the fragments of the quad as the alpha is set to 0 on it.
          glDisable(GL_ALPHA_TEST);
          }

        glBegin(GL_QUADS);
        glColor4d(this->Background[0],this->Background[1],this->Background[2],
                  0.0);
        glTexCoord2f(0.0, 0.0);
        glVertex2f(0.0, 0.0);

        glTexCoord2f(1.0, 0.0);
        glVertex2f(1.0, 0);

        //bottom vertices
        glColor4d(this->Background2[0],this->Background2[1],
                  this->Background2[2],0.0);
        glTexCoord2f(1.0, 1.0);
        glVertex2f(1.0, 1.0);

        glTexCoord2f(0.0, 1.0);
        glVertex2f(0.0, 1.0);

        glEnd();
      }
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
    }
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
    }
  vtkOpenGLCheckErrorMacro("failed after Clear");
}

//----------------------------------------------------------------------------
void vtkExternalOpenGLRenderer::PrintSelf(ostream &os, vtkIndent indent)
{
  os << indent << "ClearBuffers: " << this->ClearBuffers << "\n";
  this->Superclass::PrintSelf(os, indent);
}
