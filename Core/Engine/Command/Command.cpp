#include "Command.h"

void Command::SetObject(std::shared_ptr<StaticObject> ob) {
	m_pObject = ob;
}

std::shared_ptr<StaticObject> Command::Object() {
	return m_pObject;
}
