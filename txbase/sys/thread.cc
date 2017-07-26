#include "txbase/stdafx.h"
#include "thread.h"
#include "memory.h"

namespace TX
{
	// Get a task from the scheduler and run it
	void TaskScheduler::Worker::WorkLoop(){
		Task task;
		while(scheduler_->Running()){
			{// critical region
				std::unique_lock<std::mutex> tasks_lock(scheduler_->tasks_mutex_);

				while (scheduler_->Running() && scheduler_->tasks.empty()){
					scheduler_->task_available_cv_.wait(tasks_lock);
				}
				if (!scheduler_->Running()) return;

				// take the oldest task
				task = scheduler_->tasks.front();
				scheduler_->tasks.pop_front();
			}

			task.Run(id_);

			if (--scheduler_->task_count_ == 0){
				// only ThreadScheduler->JoinAll waits for this condition
				scheduler_->task_finished_cv_.notify_one();
			}
		}
	}

	TaskScheduler* TaskScheduler::instance = nullptr;
	TaskScheduler* TaskScheduler::Instance(){
		if (!instance){
			instance = new TaskScheduler;
		}
		return instance;
	}
	void TaskScheduler::DeleteInstance(){
		if (instance){
			instance->StopAll();
			MemDelete(instance);
		}
	}
	void TaskScheduler::StartAll(){
		int processor_count = std::thread::hardware_concurrency();
#ifdef _DEBUG
		processor_count = 1;
#endif
		running_ = true;
		for (int i = 0; i < processor_count; i++){
			threads.emplace_back(this, i);
		}
	}

	void TaskScheduler::StopAll(){
		running_ = false;
		{
			// wake up all workers so they can stop by themselves
			std::unique_lock<std::mutex> lock(tasks_mutex_);
			task_available_cv_.notify_all();
		}
		for (uint32_t i = 0; i < threads.size(); i++){
			threads[i].join();
		}
		threads.clear();
	}
	void TaskScheduler::AddTask(Task& newTask) {
		std::unique_lock<std::mutex> lock(tasks_mutex_);
		tasks.push_back(newTask);
		task_count_++;
		task_available_cv_.notify_one();
	}
	void TaskScheduler::JoinAll(){
		std::unique_lock<std::mutex> lock(finished_mutex_);

		while (task_count_ > 0){
			task_finished_cv_.wait(lock);
		}
	}
}
