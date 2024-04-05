/**
 * @file        tripletree.cpp
 * @description Student-implemented functions of a ternary tree for CPSC 221 PA3.
 *
 *              THIS FILE WILL BE SUBMITTED FOR GRADING
 */

#include "tripletree.h"

 /**
      * Constructor that builds a TripleTree out of the given PNG.
      *
      * The TripleTree represents the subimage from (0,0) to (w-1, h-1) where
      * w-1 and h-1 are the largest valid image coordinates of the original PNG.
      * Every node corresponds to a rectangle of pixels in the original PNG,
      * represented by an (x,y) pair for the upper left corner of the
      * square and two integers for the number of pixels on the width and
      * height dimensions of the rectangular region the node defines.
      *
      * The node's three children correspond to a partition
      * of the node's rectangular region into three approximately equal-size strips.
      *
      * If the rectangular region is taller than it is wide, the region is divided
      * into horizontal strips:
      *  +-------+
      *  |   A   |
      *  |       |
      *  +-------+
      *  |   B   |
      *  |       |    (Split tall)
      *  +-------+
      *  |   C   |
      *  |       |
      *  +-------+
      *
      * If the rectangular region is wider than it is tall, the region is divided
      * into vertical strips:
      *  +---------+---------+---------+
      *  |    A    |    B    |    C    |
      *  |         |         |         |    (Split wide)
      *  +---------+---------+---------+
      *
      * Your regions are not guaranteed to have dimensions exactly divisible by 3.
      * If the dimensions of your rectangular region are 3p x q or q x 3p where 3p
      * represents the length of the long side, then your rectangular regions will
      * each have dimensions p x q (or q x p)
      *
      * If the dimensions are (3p+1) x q, subregion B gets the extra pixel of size
      * while subregions A and C have dimensions p x q.
      *
      * If the dimensions are (3p+2) x q, subregions A and C each get an extra pixel
      * of size, while subregion B has dimensions p x q.
      *
      * If the region to be divided is a square, then apply the Split wide behaviour.
      *
      * Every leaf in the constructed tree corresponds to a pixel in the PNG.
      *
      * @param imIn - the input image used to construct the tree
      */
TripleTree::TripleTree(PNG& imIn) {
    root = BuildNode(imIn, {0, 0}, imIn.width(), imIn.height());
}

/**
 * Render returns a PNG image consisting of the pixels
 * stored in the tree. It may be used on pruned trees. Draws
 * every leaf node's rectangle onto a PNG canvas using the
 * average color stored in the node.
 *
 * You may want a recursive helper function for this.
 */
PNG TripleTree::Render() const {
    if (this->root == nullptr) {
        return PNG(); // Return an empty PNG if the tree is empty
    }

    PNG image(this->root->width, this->root->height);
    renderTree(image, this->root);
    return image;
}

/*
 * Prune function trims subtrees as high as possible in the tree.
 * A subtree is pruned (cleared) if all of its leaves are within
 * tol of the average color stored in the root of the subtree.
 * Pruning criteria should be evaluated on the original tree, not
 * on a pruned subtree. (we only expect that trees would be pruned once.)
 *
 * You may want a recursive helper function for this.
 *
 * @param tol - maximum allowable RGBA color distance to qualify for pruning
 */
void TripleTree::Prune(double tol) {
    pruneNode(root, tol);
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be mirrored horizontally (flipped over a vertical axis).
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::FlipHorizontal() {
    // add your implementation below
	
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be rotated 90 degrees counter-clockwise.
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::RotateCCW() {
    // add your implementation below
	
}

/*
 * Returns the number of leaf nodes in the tree.
 *
 * You may want a recursive helper function for this.
 */
int TripleTree::NumLeaves() const {
    return countLeaves(root);
}

/**
     * Destroys all dynamically allocated memory associated with the
     * current TripleTree object. To be completed for PA3.
     * You may want a recursive helper function for this one.
     */
void TripleTree::Clear() {
    clearNode(root);
}

/**
 * Copies the parameter other TripleTree into the current TripleTree.
 * Does not free any memory. Called by copy constructor and operator=.
 * You may want a recursive helper function for this one.
 * @param other - The TripleTree to be copied.
 */
void TripleTree::Copy(const TripleTree& other) {
    if (this != &other) {
        Clear();
        root = deepCopy(other.root);
    }
}

/**
 * Private helper function for the constructor. Recursively builds
 * the tree according to the specification of the constructor.
 * @param im - reference image used for construction
 * @param ul - upper left point of node to be built's rectangle.
 * @param w - width of node to be built's rectangle.
 * @param h - height of node to be built's rectangle.
 */
Node* TripleTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
    if (w == 0 || h == 0) return nullptr;

    Node* node = new Node(ul, w, h);

    if (w == 1 && h == 1) {
        node->avg = *im.getPixel(ul.first, ul.second);
        return node;
    }

    unsigned int widths[3] = {w / 3, w / 3, w / 3};
    unsigned int heights[3] = {h / 3, h / 3, h / 3};
    for (unsigned int i = 0; i < w % 3; ++i) widths[i]++;
    for (unsigned int i = 0; i < h % 3; ++i) heights[i]++;

    pair<unsigned int, unsigned int> ulA = ul;
    pair<unsigned int, unsigned int> ulB = (w >= h) ? make_pair(ul.first + widths[0], ul.second) : make_pair(ul.first, ul.second + heights[0]);
    pair<unsigned int, unsigned int> ulC = (w >= h) ? make_pair(ul.first + widths[0] + widths[1], ul.second) : make_pair(ul.first, ul.second + heights[0] + heights[1]);

    if (w >= h) {
        node->A = BuildNode(im, ulA, widths[0], h);
        node->B = BuildNode(im, ulB, widths[1], h);
        node->C = BuildNode(im, ulC, widths[2], h);
    } else {
        node->A = BuildNode(im, ulA, w, heights[0]);
        node->B = BuildNode(im, ulB, w, heights[1]);
        node->C = BuildNode(im, ulC, w, heights[2]);
    }

    calculateAvgColor(node); // Assume this function exists and updates the node's average color.
    return node;
}


/* ===== IF YOU HAVE DEFINED PRIVATE MEMBER FUNCTIONS IN tripletree_private.h, IMPLEMENT THEM HERE ====== */
void TripleTree::calculateAvgColor(Node* node) {
    long sumR = 0, sumG = 0, sumB = 0, sumA = 0;
    int totalArea = std::max(node->width, node->height);

    auto accumulate = [&](Node* child, int area) {
        if (child) {
            sumR += child->avg.r * area;
            sumG += child->avg.g * area;
            sumB += child->avg.b * area;
            sumA += child->avg.a * area;
        }
    };

    int dividedA = std::max(node->width / 3, 1);
    int dividedB = std::max(node->height / 3, 1);

    accumulate(node->A, dividedA * node->height);
    if (node->B) accumulate(node->B, dividedA * dividedB);
    accumulate(node->C, dividedA * node->height);

    node->avg = RGBAPixel(sumR / totalArea, sumG / totalArea, sumB / totalArea, sumA / totalArea);
}



void TripleTree::accumulateColor(Node* child, int& totalR, int& totalG, int& totalB, int& totalA) {
    if (!child) return;
    int area = child->width * child->height;
    totalR += child->avg.r * area;
    totalG += child->avg.g * area;
    totalB += child->avg.b * area;
    totalA += child->avg.a * area;
}

void TripleTree::renderTree(PNG& image, Node* node) const {
    if (!node) return; 

    if (!node->A && !node->B && !node->C) {
        fillPixels(image, node);
    } else {
        // Internal node: recurse into child nodes
        if (node->A) renderTree(image, node->A);
        if (node->B) renderTree(image, node->B);
        if (node->C) renderTree(image, node->C);
    }
}

void TripleTree::fillPixels(PNG& image, Node* node) const {
    for (unsigned int x = node->upperleft.first; x < node->upperleft.first + node->width; ++x) {
        for (unsigned int y = node->upperleft.second; y < node->upperleft.second + node->height; ++y) {
            *image.getPixel(x, y) = node->avg;
        }
    }
}

void TripleTree::clearNode(Node*& node) {
    if (node == nullptr) return;

    clearNode(node->A);
    clearNode(node->B);
    clearNode(node->C);

    delete node;
    node = nullptr;
}

Node* TripleTree::deepCopy(const Node* source) {
    if (!source) return nullptr;

    Node* newNode = new Node(source->upperleft, source->width, source->height);
    newNode->avg = source->avg;
    newNode->A = deepCopy(source->A);
    newNode->B = deepCopy(source->B);
    newNode->C = deepCopy(source->C);

    return newNode;
}

int TripleTree::countLeaves(const Node* node) const {
    if (!node) return 0;
    if (!node->A && !node->B && !node->C) return 1;
    return countLeaves(node->A) + countLeaves(node->B) + countLeaves(node->C);
}

bool TripleTree::shouldPrune(const Node* node, const RGBAPixel& avg, double tol) const {
    if (!node) return true;
    if (!node->A && !node->B && !node->C) {
        RGBAPixel tempAvg = node->avg;
        return tempAvg.distanceTo(avg) <= tol;
    }
    return shouldPrune(node->A, avg, tol) && shouldPrune(node->B, avg, tol) && shouldPrune(node->C, avg, tol);
}

void TripleTree::pruneNode(Node*& node, double tol) {
    if (!node) return;
    if (shouldPrune(node, node->avg, tol)) {
        clearNode(node->A);
        clearNode(node->B);
        clearNode(node->C);
    } else {
        pruneNode(node->A, tol);
        pruneNode(node->B, tol);
        pruneNode(node->C, tol);
    }
}