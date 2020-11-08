#include "pch.h"
#include "Utility.hpp"
#include "Scene.hpp"

//*******************************************************************
//Scene
//*******************************************************************
Scene::Scene() {
	frame_ = 0U;
}
Scene::~Scene() {
}
void Scene::Render() {
	for (shared_ptr<TaskBase>& iTask : listTask_) {
		if (iTask && !iTask->IsFinished())
			iTask->Render();
	}
}
void Scene::Update() {
	for (auto itr = listTask_.begin(); itr != listTask_.end();) {
		shared_ptr<TaskBase> task = *itr;
		if (task && !task->IsFinished()) {
			task->Update();
			if (task->GetFrame() >= task->GetFrameEnd())
				task->bFinish_ = true;
			++itr;
		}
		else itr = listTask_.erase(itr);
	}
	++frame_;
}
std::list<shared_ptr<TaskBase>>::iterator Scene::AddTask(shared_ptr<TaskBase> task) {
	listTask_.push_back(task);
	return listTask_.rbegin().base();
}
std::list<shared_ptr<TaskBase>>::iterator Scene::AddTask(std::list<shared_ptr<TaskBase>>::iterator itr, shared_ptr<TaskBase> task) {
	auto itrRes = listTask_.insert(itr, task);
	return itrRes;
}

//*******************************************************************
//TaskBase
//*******************************************************************
TaskBase::TaskBase(Scene* parent) {
	parent_ = parent;
	frame_ = 0;
	frameEnd_ = UINT_MAX;
	bFinish_ = false;
}