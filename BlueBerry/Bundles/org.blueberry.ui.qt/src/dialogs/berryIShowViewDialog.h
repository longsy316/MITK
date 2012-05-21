/*===================================================================

BlueBerry Platform

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef BERRYISHOWVIEWDIALOG_H_
#define BERRYISHOWVIEWDIALOG_H_

#include "berryIDialog.h"

#include <vector>
#include "berryIViewDescriptor.h"

namespace berry {

/**
 * \ingroup org_blueberry_ui_qt
 *
 */
struct BERRY_UI_QT IShowViewDialog : public IDialog
{
  berryInterfaceMacro(IShowViewDialog, berry);

  ~IShowViewDialog();


  virtual QList<IViewDescriptor::Pointer> GetSelection() = 0;

};

}

#endif /*BERRYISHOWVIEWDIALOG_H_*/
