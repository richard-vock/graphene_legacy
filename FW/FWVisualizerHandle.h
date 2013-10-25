#ifndef FWVISUALIZERHANDLE_H_
#define FWVISUALIZERHANDLE_H_

/**
 *  @file FWVisualizerHandle.h
 *
 *  @brief Defines access class to framework functionality provided to visualizers.
 */

#include <include/common.h>

#include <Geometry/Ray.h>
#include <FW/View/Transforms.h>
#include <FW/Events/Handle.h>
#include <FW/Events/Modifier.h>
#include <FW/Events/Keys.h>
using FW::Events::Keys;

namespace FW {

class Graphene;

/**
 *  Access class to framework functionality provided to visualizers.
 *
 *  In any visualizers (i.e. classes inheriting from Visualizer)
 *  this handle can be accessed using the fw() member function.
 *
 *  @see GUI::VisualizerHandle
 */
class VisualizerHandle {
	public:
		/** Shared pointer to this class */
		typedef std::shared_ptr<VisualizerHandle> Ptr;
		/** Weak pointer to this class */
		typedef std::weak_ptr<VisualizerHandle>   WPtr;
		friend class Graphene;

	protected:
		VisualizerHandle(std::string id, View::Transforms::WPtr transforms, Events::EventHandler::Ptr eventHandler, Geometry::Ray::Ptr pickRay);

	public:
		/**
		 *  Destructor
		 */
		virtual ~VisualizerHandle();

		/**
		 *  Returns access class to OpenGL transform parameters.
		 *  
		 *  @see View::Transforms
		 */
		View::Transforms::Ptr  transforms();

		/**
		 *  Returns access class to event management system.
		 *  
		 *  @see Events::Handle
		 */
		Events::Handle::Ptr    events();

		/**
		 *  Returns access class to keyboard modifier states.
		 *  
		 *  @see Events::Modifier
		 */
		Events::Modifier::Ptr  modifier();

		/**
		 *  Returns access class to current pick ray.
		 */
		Geometry::Ray::Ptr     pickRay();

	protected:
		std::string            m_id;
		View::Transforms::WPtr m_transforms;
		Events::Handle::Ptr    m_events;
		Events::Modifier::Ptr  m_modifier;
		Geometry::Ray::Ptr     m_pickRay;
};

} // FW

#endif /* FWVISUALIZERHANDLE_H_ */
