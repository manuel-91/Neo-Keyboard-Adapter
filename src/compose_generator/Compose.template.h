#ifndef __COMPOSE_H__
#define __COMPOSE_H__

#include "NeoIncludes.h"
#include <assert.h>

/*
 * The tree is static, therefore no methods for removing/adding elements,
 * to not make life too complicated, this file is generated by a script.
 * 
 * The implementation with bit fields and unions is not the most pretty one
 * but the most memory efficient.
 * 
 * Payload is either pointing to next node or contains unicode symbol.
 * indication what is inside is given by the isLeaf flag cast to uint16_t
 * in Initialization only to trick the compiler, as only first element of the
 * union can be initialized. 
*/

struct LayeredKey {
	uint8_t layer;
	uint8_t key;
};

struct Node {
	struct {
		uint8_t isLastElement : 1;
		uint8_t isLeaf : 1;
		uint8_t layer : 6;
		uint8_t keycode : 8;
	} layeredKey;
	union {
		uint16_t unicode;
		Node* nodes;
	} payload;
};


class Compose
{
	private:
	Node* currentNode;

<GENERATOR>

	const static LayeredKey aliases[][2];
	
	LayeredKey getAlias(uint8_t layer, uint8_t key);

	public:
	Compose() : currentNode(const_cast<Node*>(root)){
		//if this assertion fails, the struct in Node has to be reworked for the target architecture
		assert(sizeof(uint16_t) >= sizeof(Node*));
	}
	
	uint16_t transition(uint8_t layer, uint8_t key);
	void reset();
	
	


}; //Compose

#endif //__COMPOSE_H__