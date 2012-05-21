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

#ifndef BERRYWORKBENCHPARTREFERENCE_H_
#define BERRYWORKBENCHPARTREFERENCE_H_

#include <berryMessage.h>

#include "berryISizeProvider.h"
#include "berryIWorkbenchPartReference.h"
#include "berryIWorkbenchPart.h"

#include "berryImageDescriptor.h"

namespace berry {

class PartPane;

/**
 * \ingroup org_blueberry_ui_internal
 *
 */
class BERRY_UI_QT WorkbenchPartReference : virtual public IWorkbenchPartReference, public ISizeProvider {

public:

  berryObjectMacro(WorkbenchPartReference)


    // State constants //////////////////////////////

    /**
     * State constant indicating that the part is not created yet
     */
public: static int STATE_LAZY; // = 0

    /**
     * State constant indicating that the part is in the process of being created
     */
public: static int STATE_CREATION_IN_PROGRESS; // = 1

    /**
     * State constant indicating that the part has been created
     */
public: static int STATE_CREATED; // = 2

    /**
     * State constant indicating that the reference has been disposed (the reference shouldn't be
     * used anymore)
     */
public: static int STATE_DISPOSED; // = 3

    /**
     * Current state of the reference. Used to detect recursive creation errors, disposed
     * references, etc.
     */
private: int state;

protected: IWorkbenchPart::Pointer part;

protected: SmartPointer<PartPane> pane;

private: QString id;

private: bool pinned;

private: QString tooltip;

/**
 * Stores the current Image for this part reference. Lazily created. Null if not allocated.
 */
private: void* image;

private: SmartPointer<ImageDescriptor> defaultImageDescriptor;

    /**
     * Stores the current image descriptor for the part.
     */
private: SmartPointer<ImageDescriptor> imageDescriptor;

    /**
     * API listener list
     */
private: IPropertyChangeListener::Events propChangeEvents;

//private: ListenerList partChangeListeners = new ListenerList();

private: QString partName;

private: QString contentDescription;

protected: QHash<QString, QString> propertyCache;

    /**
     * Used to remember which events have been queued.
     */
private: QSet<int> queuedEvents;

private: bool queueEvents;

//private: static DisposeListener prematureDisposeListener = new DisposeListener() {
//        public void widgetDisposed(DisposeEvent e) {
//            WorkbenchPlugin.log(new RuntimeException("Widget disposed too early!")); //$NON-NLS-1$
//        }
//    };

private: struct PropertyChangeListener : public IPropertyChangeListener
   {
  PropertyChangeListener(WorkbenchPartReference* ref);
     void PropertyChange(PropertyChangeEvent::Pointer event);

   private: WorkbenchPartReference* partRef;
    };

private: IPropertyChangeListener::Pointer propertyChangeListener;

/**
 * Calling this with deferEvents(true) will queue all property change events until a subsequent
 * call to deferEvents(false). This should be used at the beginning of a batch of related changes
 * to prevent duplicate property change events from being sent.
 *
 * @param shouldQueue
 */
private: void DeferEvents(bool shouldQueue);

public: WorkbenchPartReference();

public: ~WorkbenchPartReference();

public: virtual bool IsDisposed() const;

protected: virtual void CheckReference();

    /**
     * Calling this with deferEvents(true) will queue all property change events until a subsequent
     * call to deferEvents(false). This should be used at the beginning of a batch of related changes
     * to prevent duplicate property change events from being sent.
     *
     * @param shouldQueue
     */
//private: virtual void DeferEvents(bool shouldQueue);

protected: virtual void SetPartName(const QString& newPartName);

protected: virtual void SetContentDescription(const QString& newContentDescription);

protected: virtual void SetImageDescriptor(SmartPointer<ImageDescriptor> descriptor);

protected: virtual void SetToolTip(const QString& newToolTip);

protected: virtual void PropertyChanged(Object::Pointer source, int propId);

protected: virtual void PropertyChanged(PropertyChangeEvent::Pointer event);

    /**
     * Refreshes all cached values with the values from the real part
     */
protected: virtual void RefreshFromPart();

protected: virtual SmartPointer<ImageDescriptor> ComputeImageDescriptor();

public: virtual void Init(const QString& id, const QString& tooltip,
            SmartPointer<ImageDescriptor> desc, const QString& paneName, const QString& contentDescription);


    /**
     * @see IWorkbenchPart
     */
public: virtual void AddPropertyListener(IPropertyChangeListener::Pointer listener);

    /**
     * @see IWorkbenchPart
     */
public: virtual void RemovePropertyListener(IPropertyChangeListener::Pointer listener);

public: QString GetId() const;

public: virtual QString GetTitleToolTip() const;

protected: QString GetRawToolTip() const;

    /**
     * Returns the pane name for the part
     *
     * @return the pane name for the part
     */
public: virtual QString GetPartName() const;

    /**
     * Gets the part name directly from the associated workbench part,
     * or the empty string if none.
     *
     * @return
     */
protected: QString GetRawPartName() const;

protected: virtual QString ComputePartName() const;

    /**
     * Returns the content description for this part.
     *
     * @return the pane name for the part
     */
public: virtual QString GetContentDescription() const;

    /**
     * Computes a new content description for the part. Subclasses may override to change the
     * default behavior
     *
     * @return the new content description for the part
     */
protected: virtual QString ComputeContentDescription() const;

    /**
     * Returns the content description as set directly by the part, or the empty string if none
     *
     * @return the unmodified content description from the part (or the empty string if none)
     */
protected: QString GetRawContentDescription() const;

public: virtual bool IsDirty() const;

public: virtual void* GetTitleImage();

public: virtual SmartPointer<ImageDescriptor> GetTitleImageDescriptor() const;

public: virtual void FireVisibilityChange();

//    /* package */ virtual void fireZoomChange();

public: virtual bool GetVisible();

public: virtual void SetVisible(bool isVisible);

protected: virtual void FirePropertyChange(int id);

private: void ImmediateFirePropertyChange(int id);

public: IWorkbenchPart::Pointer GetPart(bool restore);

protected: virtual IWorkbenchPart::Pointer CreatePart() = 0;

protected: virtual SmartPointer<PartPane> CreatePane() = 0;

    /**
     * Returns the part pane for this part reference. Does not return null.
     *
     * @return
     */
public: SmartPointer<PartPane> GetPane();

public: void Dispose();

  /**
   * Clears all of the listeners in a listener list. TODO Bug 117519 Remove
   * this method when fixed.
   *
   * @param list
   *            The list to be clear; must not be <code>null</code>.
   */
//private: void clearListenerList(const ListenerList list);


public: virtual void SetPinned(bool newPinned);

public: virtual bool IsPinned() const;

protected: void DoDisposePart();

/* (non-Javadoc)
 * @see org.blueberry.ui.IWorkbenchPartReference#getPartProperty(java.lang.String)
 */
public: virtual QString GetPartProperty(const QString& key) const;

protected: virtual void FirePropertyChange(PropertyChangeEvent::Pointer event);

protected: virtual void CreatePartProperties(IWorkbenchPart::Pointer workbenchPart);

public: int ComputePreferredSize(bool width, int availableParallel,
            int availablePerpendicular, int preferredResult);

public: int GetSizeFlags(bool width);

};

} // namespace berry

#endif /*BERRYWORKBENCHPARTREFERENCE_H_*/
