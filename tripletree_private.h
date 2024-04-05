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
void renderTree(PNG& im, Node* node) const;
void pruneHelper(Node* node, RGBAPixel& color, double tol);
void flipHorizontally(Node* node);
void rotateCounterClockwise(Node* node);
void swapDimensions(Node* node);
int countLeaves(Node* node) const;
void clearNode(Node*& node);
Node* copyTree(Node* other);
void computeAvgColor(Node* node);
double nodeColorDistance(const RGBAPixel &nodeColor, const RGBAPixel &targetColor) const;
// double maxChildDist(Node* node, RGBAPixel& color) const;
bool shouldPrune(const Node* node, const RGBAPixel& avg, double tol) const;
void pruneNode(Node*& node, double tol);