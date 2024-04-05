/**
 * @file        tripletree_private.h
 * @description student declarations of private functions
 *              for TripleTree, CPSC 221 PA3
 *
 *              THIS FILE WILL BE SUBMITTED.
 *
 *              Simply declare your function prototypes here.
 *              No other scaffolding is necessary.
 */

 // begin your declarations below
void renderTree(PNG& image, Node* node) const;
void fillPixels(PNG& image, Node* node) const;
void calculateAvgColor(Node* current);
void accumulateColor(Node* child, int& totalR, int& totalG, int& totalB, int& totalA);

void clearNode(Node*& node);
Node* deepCopy(const Node* source);
int countLeaves(const Node* node) const;
bool shouldPrune(const Node* node, const RGBAPixel& avg, double tol) const;
void pruneNode(Node*& node, double tol);
double nodeColorDistance(const RGBAPixel &nodeColor, const RGBAPixel &targetColor) const;