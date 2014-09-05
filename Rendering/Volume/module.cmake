if (Module_vtkVolumeOpenGL2)
  set(VTK_RENDERING_VOLUME_BACKEND "vtkRenderingVolumeOpenGL;vtkVolumeOpenGL2")
else()
  set(VTK_RENDERING_VOLUME_BACKEND "vtkRenderingVolumeOpenGL")
endif()

vtk_module(vtkRenderingVolume
  GROUPS
    Rendering
  DEPENDS
    vtkImagingCore
    vtkRenderingCore
  TEST_DEPENDS
    vtkTestingCore
    vtkTestingRendering
    ${VTK_RENDERING_VOLUME_BACKEND}
    vtkRenderingFreeType
    vtkIOXML
    vtkImagingSources
    vtkImagingGeneral
    vtkImagingHybrid
    vtkInteractionStyle
    vtkIOLegacy
  KIT
    vtkRendering
  )
