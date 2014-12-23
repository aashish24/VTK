/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestGPURayCastIndependentComponents.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// This is a test for the independent components functionality of the
// GPU volume mapper. It creates an image data with 4 independent components.
// The image data is a cube with each component representing 4 different
// quadrants in space. The colormap is set to color each component quadrant
// differently.

#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageData.h>
#include <vtkNew.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRegressionTestImage.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTesting.h>
#include <vtkVolumeProperty.h>

int TestGPURayCastIndependentComponents(int argc, char * argv[])
{
  // Create a 4 component image data
  vtkNew<vtkImageData> image;
  image->SetDimensions(10, 10, 10);
  image->AllocateScalars(VTK_DOUBLE, 4);

  // Fill every entry of the image data
  int * dims = image->GetDimensions();
  double * ptr = static_cast<double *> (image->GetScalarPointer(0, 0, 0));
  for (int z = 0; z < dims[2]; ++z)
    {
    for (int y = 0; y < dims[1]; ++y)
      {
      for (int x = 0; x < dims[0]; ++x)
        {
        if (x < dims[0]/2 && y < dims[1]/2)
          {
          *ptr++ = 1.0;
          *ptr++ = 0.0;
          *ptr++ = 0.0;
          *ptr++ = 0.0;
          }
        else if (x >= dims[0]/2 && y < dims[1]/2)
          {
          *ptr++ = 0.0;
          *ptr++ = 1.0;
          *ptr++ = 0.0;
          *ptr++ = 0.0;
          }
        else if (x < dims[0]/2 && y > dims[1]/2)
          {
          *ptr++ = 0.0;
          *ptr++ = 0.0;
          *ptr++ = 1.0;
          *ptr++ = 0.0;
          }
        else
          {
          *ptr++ = 0.0;
          *ptr++ = 0.0;
          *ptr++ = 0.0;
          *ptr++ = 1.0;
          }
        }
      }
    }

  vtkNew<vtkRenderWindow> renWin;
  renWin->SetSize(400, 401);
  renWin->SetMultiSamples(0);
  vtkNew<vtkRenderer> ren;
  renWin->AddRenderer(ren.GetPointer());
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin.GetPointer());

  vtkNew<vtkGPUVolumeRayCastMapper> mapper;
  mapper->SetInputData(image.GetPointer());

  vtkNew<vtkColorTransferFunction> ctf1;
  ctf1->AddRGBPoint(0.0, 1.0, 1.0, 1.0);
  ctf1->AddRGBPoint(1.0, 1.0, 0.0, 0.0);

  vtkNew<vtkColorTransferFunction> ctf2;
  ctf2->AddRGBPoint(0.0, 1.0, 1.0, 1.0);
  ctf2->AddRGBPoint(1.0, 0.0, 1.0, 0.0);

  vtkNew<vtkColorTransferFunction> ctf3;
  ctf3->AddRGBPoint(0.0, 1.0, 1.0, 1.0);
  ctf3->AddRGBPoint(1.0, 0.0, 0.0, 1.0);

  vtkNew<vtkColorTransferFunction> ctf4;
  ctf4->AddRGBPoint(0.0, 1.0, 1.0, 1.0);
  ctf4->AddRGBPoint(1.0, 1.0, 0.0, 1.0);

  vtkNew<vtkVolumeProperty> property;
  property->SetColor(0, ctf1.GetPointer());
  property->SetColor(1, ctf2.GetPointer());
  property->SetColor(2, ctf3.GetPointer());
  property->SetColor(3, ctf4.GetPointer());

  vtkNew<vtkVolume> volume;
  volume->SetMapper(mapper.GetPointer());
  volume->SetProperty(property.GetPointer());
  ren->AddVolume(volume.GetPointer());

  ren->ResetCamera();
  renWin->Render();

  iren->Initialize();

  int retVal = vtkRegressionTestImage(renWin.GetPointer());
  if (retVal == vtkRegressionTester::DO_INTERACTOR)
    {
    iren->Start();
    }

  return !retVal;
}
