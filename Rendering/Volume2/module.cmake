vtk_module(vtkVolume
  DEPENDS
    vtkCommonDataModel
    vtkCommonExecutionModel
    vtkRenderingVolume
    vtkRenderingOpenGL
  TEST_DEPENDS
    vtkTestingCore
    vtkRenderingOpenGL
    vtkTestingRendering
    vtkInteractionStyle
)
