vtk_add_test_cxx(
  TestGDALVectorReader.cxx
  TestGDALRasterReader.cxx
  )
vtk_test_cxx_executable(${vtk-module}CxxTests)
