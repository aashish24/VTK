/*=========================================================================

  Program:   Visualization Toolkit
  Module:    raycasterfs.glsl

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#version 120

// The following line handle system declarations such a
// default precisions, or defining precisions to null
//VTK::System::Dec

//////////////////////////////////////////////////////////////////////////////
///
/// Inputs
///
//////////////////////////////////////////////////////////////////////////////

/// 3D texture coordinates form vertex shader
varying vec3 v_textureCoords;
varying vec3 v_vertexPos;

//////////////////////////////////////////////////////////////////////////////
///
/// Outputs
///
//////////////////////////////////////////////////////////////////////////////

vec4 g_fragColor;

//////////////////////////////////////////////////////////////////////////////
///
/// Uniforms, attributes, and globals
///
//////////////////////////////////////////////////////////////////////////////
vec3 g_dataPos;
vec3 g_dirStep;

//VTK::Base::Dec
//VTK::Termination::Dec
//VTK::Cropping::Dec
//VTK::Shading::Dec
//VTK::BinaryMask::Dec
//VTK::CompositeMask::Dec

//VTK::ComputeOpacity::Dec
//VTK::ComputeGradient::Dec
//VTK::ComputeLighting::Dec
//VTK::ColorTransferFunc::Dec

//VTK::RayDirectionFunc::Dec

/// We support only 8 clipping planes for now
/// The first value is the size of the data array for clipping
/// planes (origin, normal)
uniform float in_clippingPlanes[49];

//////////////////////////////////////////////////////////////////////////////
///
/// Main
///
//////////////////////////////////////////////////////////////////////////////
void main()
{
  /// Initialize g_fragColor (output) to 0
  g_fragColor = vec4(0.0);
  g_dirStep = vec3(0.0);

  //VTK::Base::Init
  //VTK::Terminate::Init
  //VTK::Shading::Init
  //VTK::Cropping::Init
  //VTK::Clipping::Init

  /// For all samples along the ray
  while (true)
    {
    //VTK::Base::Impl
    //VTK::Terminate::Impl
    //VTK::Cropping::Impl
    //VTK::Clipping::Impl
    //VTK::BinaryMask::Impl
    //VTK::CompositeMask::Impl
    //VTK::Shading::Impl

    /// Advance ray by m_dir_step
    g_dataPos += g_dirStep;
    }

  //VTK::Base::Exit
  //VTK::Terminate::Exit
  //VTK::Cropping::Exit
  //VTK::Clipping::Exit
  //VTK::Shading::Exit

  gl_FragColor = g_fragColor;
}
