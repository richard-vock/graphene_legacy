#ifndef FWVISUALIZER_H_
#define FWVISUALIZER_H_

/**
 *  @file Visualizer.h
 *
 *  @brief Defines base class for custom visualizers
 *
 */

#include <include/common.h>
#include <include/ogl.h>
#include <future>

#include <FW/FWVisualizerHandle.h>
#include <GUI/GUIVisualizerHandle.h>

#include <IO/AbstractProgressBarPool.h>

namespace FW {

class Graphene;

/**
 *  @brief Base class for custom visualizers
 *
 *  This class serves as an abstract base class for actual visualizers.
 *  Inherited class should define init() and render() member functions.
 *  For examples see the visualizer examples.
 */
class Visualizer {
	public:
		/** shared pointer to this class */
		typedef std::shared_ptr<Visualizer> Ptr;
		/** weak pointer to this class */
		typedef std::weak_ptr<Visualizer> WPtr;
		/** defines a function to process in a separate thread */
		typedef std::function<void (void)>    Job;
		/** defines a function to process in a separate thread, provided a progress bar */
		typedef std::function<void (IO::AbstractProgressBar::Ptr)>        JobWithBar;
//		typedef std::function<void (IO::AbstractProgressBarPool::Ptr)>    JobWithPool;
		friend class Graphene;

	public:
		/**
		 *  Constructor
		 *
		 *  @param id Identifier of visualizer. Inherited classes should ignore this id (delegate to base constructor)
		 */
		Visualizer(std::string id);

		/**
		 *  Destructor
		 */
		virtual ~Visualizer();

		/**
		 *  Returns identifier of visualizer (usually the name)
		 *
		 *  @return Identifier
		 */
		std::string id() const;

		/**
		 *  Returns access handle to framework functionality
		 *  
		 *  @return Access handle to framework.
		 *  @see FW::VisualizerHandle
		 */
		FW::VisualizerHandle::Ptr  fw();

		/**
		 *  Returns access handle to GUI functionality
		 *
		 *  @return Access handel to GUI
		 *  @see GUI::VisualizerHandle
		 */
		GUI::VisualizerHandle::Ptr gui();

		/**
		 *  Pure virtual method initializing inherited visualizers.
		 */
		virtual void init() = 0;

		/**
		 *  Pure virtual method rendering content of inherited visualizers.
		 */
		virtual void render() = 0;

		/**
		 *  Execute given function in a separate thread.
		 *
		 *  @param task Callback function to execute independently.
		 *  @param finally Callback function to call after successfully executing task.
		 */
		void execute(Job task, Job finally);

		/**
		 *  Execute given function in a separate thread, provided a progress bar.
		 *
		 *  @param task Callback function to execute independently (function is given a progress bar).
		 *  @param finally Callback function to call after successfully executing task.
		 *  @param taskName Label for task used in the GUI.
		 *  @param steps (Optional) parameter specifying number of steps after which progress bar is updated.
		 */
		void execute(JobWithBar task, Job finally, std::string taskName, int steps = 1);
		//void execute(JobWithPool task, Job finally);

		/**
		 *  Assemble string vector (used in graphene as a path) out of given parameter.
		 *
		 *  Convenience function for those who don't have initializer list support.
		 *  Keep in mind that parameter is given as r-value references and std::move'd inside the function.
		 *  If necessary supply copies.
		 *  
		 *  @param s0 String to include
		 */
		static std::vector<std::string> path(std::string&& s0);

		/**
		 *  Assemble string vector (used in graphene as a path) out of given parameters.
		 *
		 *  Convenience function for those who don't have initializer list support.
		 *  Keep in mind that parameters are given as r-value references and std::move'd inside the function.
		 *  If necessary supply copies.
		 *  
		 *  @param s0 String to include
		 *  @param s1 String to include
		 */
		static std::vector<std::string> path(std::string&& s0, std::string&& s1);

		/**
		 *  Assemble string vector (used in graphene as a path) out of given parameters.
		 *
		 *  Convenience function for those who don't have initializer list support.
		 *  Keep in mind that parameters are given as r-value references and std::move'd inside the function.
		 *  If necessary supply copies.
		 *  
		 *  @param s0 String to include
		 *  @param s1 String to include
		 *  @param s2 String to include
		 */
		static std::vector<std::string> path(std::string&& s0, std::string&& s1, std::string&& s2);

	protected:
		void setHandles(FW::VisualizerHandle::Ptr fw, GUI::VisualizerHandle::Ptr gui);
		void setProgressBarPool(IO::AbstractProgressBarPool::Ptr m_pool);
		void waitForTasks();

	protected:
		typedef std::pair<std::future<void>, Job> Task;

	protected:
		std::string                       m_id;
		FW::VisualizerHandle::Ptr         m_fw;
		GUI::VisualizerHandle::Ptr        m_gui;
		std::vector<Task>                 m_tasks;
		IO::AbstractProgressBarPool::Ptr  m_pool;
};


} // FW

#endif /* FWVISUALIZER_H_ */
