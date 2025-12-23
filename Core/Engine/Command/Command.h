#pragma once
#include <memory>

class StaticObject;

class Command {
	public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
	
	public:
	void SetObject(std::shared_ptr<StaticObject> ob);
	std::shared_ptr<StaticObject> Object();

	protected:
	std::shared_ptr<StaticObject> m_pObject = nullptr;
};

