#pragma once
#include <memory>

class StaticObject;

class Command {
	public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
	
	public:
	void SetObject(std::shared_ptr<StaticObject> ob);

	protected:
	std::shared_ptr<StaticObject> m_pObject = nullptr;
};

