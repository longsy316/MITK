/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "mitkAnnotationService.h"
#include <mitkAbstractAnnotationRenderer.h>

void mitk::AnnotationService::AddAnnotationRenderer(mitk::AnnotationServiceRef ref, const std::string &id)
{
  m_InstanceMap[id] = ref;
}

mitk::AnnotationService::AnnotationService()
{
}

mitk::AnnotationService::~AnnotationService()
{
}

