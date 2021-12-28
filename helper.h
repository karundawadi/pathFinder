#ifndef HELPER_FUNCTION
#define HELPER_FUNCTION
//All the structs are typedef here 

// This is to define a vertices 
typedef struct Node{
	int x;
	int y;

	struct Node *next;
	struct Node *prev;
} Node;
// declare direction enums
enum TurnDirection{Left, Right, Around, NoDir};

enum Direction{North, South, East, West};

#endif
