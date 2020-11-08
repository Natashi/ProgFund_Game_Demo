#pragma once
#include "../../pch.h"

class Scene;
class TaskBase {
	friend class Scene;
public:
	TaskBase(Scene* parent);
	virtual ~TaskBase() {};

	virtual void Render() {};
	virtual void Update() {};

	Scene* GetParent() { return parent_; }

	void SetEndFrame(size_t frame) { frameEnd_ = frame; }

	size_t GetFrame() { return frame_; }
	size_t GetFrameEnd() { return frameEnd_; }
	bool IsFinished() { return bFinish_; }
protected:
	Scene* parent_;
	size_t frame_;
	size_t frameEnd_;
	bool bFinish_;
};

class Scene {
public:
	Scene();
	virtual ~Scene();

	virtual void Render();
	virtual void Update();

	size_t GetFrame() { return frame_; }

	size_t GetTaskCount() { return listTask_.size(); }

	std::list<shared_ptr<TaskBase>>::iterator AddTask(shared_ptr<TaskBase> task);
	std::list<shared_ptr<TaskBase>>::iterator AddTask(std::list<shared_ptr<TaskBase>>::iterator itr, shared_ptr<TaskBase> task);
protected:
	size_t frame_;
	std::list<shared_ptr<TaskBase>> listTask_;
};