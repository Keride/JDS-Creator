#include "Case.h"



Case::Case(){
}

void Case::SetNextCase(Case* pNextCase) {
	this->pNextCase = pNextCase;
}

Case* Case::getNextCase() {
	return this->pNextCase;
}

Case::Case(Zone* pZone):Objet(pZone){
	
}

Case::~Case(){
}
