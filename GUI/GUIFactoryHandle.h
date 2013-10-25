#ifndef GUIFACTORYHANDLE_H_
#define GUIFACTORYHANDLE_H_

/**
 *  @file GUIFactoryHandle.h
 *
 *  @brief Defines access class to GUI functionality provided to factories.
 */

#include <include/common.h>

#include "Property/Container.h"

namespace GUI {

/**
 *  Defines access class to GUI functionality provided to factories.
 */
class FactoryHandle {
	public:
		/** Shared pointer to this class */
		typedef std::shared_ptr<FactoryHandle> Ptr;

	public:
		/**
		 *  Constructor.
		 *
		 *  @param properties Access handle to Container implementation. Supplies access to property area for factories.
		 */
		FactoryHandle(Property::Container::Ptr properties);

		/**  Destructor. */
		virtual ~FactoryHandle();

		/**
		 *  Returns access handle to property area
		 *
		 *  @return Shared pointer to property Container.
		 */
		Property::Container::Ptr properties();

	protected:
		Property::Container::Ptr m_properties;
};


} // GUI

#endif /* GUIFACTORYHANDLE_H_ */
