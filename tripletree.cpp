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
    flipHorizontally(root);
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be rotated 90 degrees counter-clockwise.
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::RotateCCW() {
    swapDimensions(root);
    rotateCounterClockwise(root);
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
    root = NULL; 
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
        root = copyTree(other.root);
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
    if ((w == 0) || (h == 0)) {
        return nullptr;
    }

    Node* node = new Node(ul, w, h); 

    if ((w == 1) && (h == 1)) {
        node->avg = *im.getPixel(ul.first, ul.second);
        return node;
    }

    int length = (w > h) ? w : h;
    int partA = length / 3;
    int partB = partA;

    if (length % 3 == 1) {
        partB++;
    } else if (length % 3 == 2) {
        partA++;
    }

    if (w < h) {
        pair<unsigned int, unsigned int> ul_B(ul.first, ul.second + partA);
        pair<unsigned int, unsigned int> ul_C(ul.first, ul.second + partA + partB);

        node->A = BuildNode(im, ul, w, partA);
        node->B = BuildNode(im, ul_B, w, partB);
        node->C = BuildNode(im, ul_C, w, partA);

        computeAvgColor(node);
    } else {
        pair<unsigned int, unsigned int> ul_B(ul.first + partA, ul.second);
        pair<unsigned int, unsigned int> ul_C(ul.first + partA + partB, ul.second);

        node->A = BuildNode(im, ul, partA, h);
        node->B = BuildNode(im, ul_B, partB, h);
        node->C = BuildNode(im, ul_C, partA, h);
        computeAvgColor(node);
    }

    return node;
}

void TripleTree::computeAvgColor(Node* node) {
    int totalArea = node->width * node->height;
    int areaA = (node->A != nullptr) ? node->A->width * node->A->height : 0;
    int areaB = (node->B != nullptr) ? node->B->width * node->B->height : 0;
    int areaC = (node->C != nullptr) ? node->C->width * node->C->height : 0;

    char red, green, blue;
    double alpha;

    if (node->B == nullptr) {
        red = (node->A->avg.r * areaA + node->C->avg.r * areaC) / totalArea;
        green = (node->A->avg.g * areaA + node->C->avg.g * areaC) / totalArea;
        blue = (node->A->avg.b * areaA + node->C->avg.b * areaC) / totalArea;
        alpha = (node->A->avg.a * areaA + node->C->avg.a * areaC) / totalArea;
    } else {
        red = (node->A->avg.r * areaA + node->B->avg.r * areaB + node->C->avg.r * areaC) / totalArea;
        green = (node->A->avg.g * areaA + node->B->avg.g * areaB + node->C->avg.g * areaC) / totalArea;
        blue = (node->A->avg.b * areaA + node->B->avg.b * areaB + node->C->avg.b * areaC) / totalArea;
        alpha = (node->A->avg.a * areaA + node->B->avg.a * areaB + node->C->avg.a * areaC) / totalArea;
    }

    node->avg = RGBAPixel(red, green, blue, alpha);
}

void TripleTree::renderTree(PNG& im, Node* node) const {
    if (!node) return; // Base case: node is null

    if (!node->A && !node->B && !node->C) { // Leaf node
        for (unsigned x = node->upperleft.first; x < node->upperleft.first + node->width; ++x) {
            for (unsigned y = node->upperleft.second; y < node->upperleft.second + node->height; ++y) {
                *im.getPixel(x, y) = node->avg;
            }
        }
    } else { // Recursive case: has children
        renderTree(im, node->A);
        renderTree(im, node->B);
        renderTree(im, node->C);
    }
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

double TripleTree::nodeColorDistance(const RGBAPixel &nodeColor, const RGBAPixel &targetColor) const {
    return (nodeColor.r - targetColor.r) * (nodeColor.r - targetColor.r) +
           (nodeColor.g - targetColor.g) * (nodeColor.g - targetColor.g) +
           (nodeColor.b - targetColor.b) * (nodeColor.b - targetColor.b);
}

void TripleTree::flipHorizontally(Node* node) {
    if (node == nullptr) {
        return;
    }

    if (node->A && node->C) {
        if (node->height > node->width) {
            node->A->upperleft.first = node->upperleft.first;
            node->C->upperleft.first = node->upperleft.first;

            if (node->B) {
                node->B->upperleft.first = node->upperleft.first;
            }
        } else {
            Node* temp = node->A;
            node->A = node->C;
            node->C = temp;

            node->A->upperleft = node->upperleft;

            if (!node->B) {
                node->C->upperleft.first = node->upperleft.first + node->A->width;
            } else {
                node->B->upperleft.first = node->upperleft.first + node->A->width;
                node->C->upperleft.first = node->upperleft.first + node->A->width + node->B->width;
            }
        }
    }

    flipHorizontally(node->A);
    flipHorizontally(node->B);
    flipHorizontally(node->C);
}

void TripleTree::rotateCounterClockwise(Node* node) {
    if (node == nullptr || node->A == nullptr) {
        return;
    }
    
    int bMinDimension = 0;

    if (node->B != nullptr) {
        int temp = node->B->width;
        node->B->width = node->B->height;
        node->B->height = temp;
        bMinDimension = std::min(node->B->height, node->B->width);
    }

    int tempAWidth = node->A->width;
    node->A->width = node->A->height;
    node->A->height = tempAWidth;

    int tempCWidth = node->C->width;
    node->C->width = node->C->height;
    node->C->height = tempCWidth;

    if ((2 * node->A->height + bMinDimension) == node->height) {
        Node* temp = node->C;
        node->C = node->A;
        node->A = temp;

        node->A->upperleft = node->upperleft;
        if (node->B != nullptr) {
            node->B->upperleft.first = node->upperleft.first;
            node->B->upperleft.second = node->upperleft.second + node->A->height;
        }
        node->C->upperleft.first = node->upperleft.first;
        node->C->upperleft.second = node->upperleft.second + node->A->height + bMinDimension;

    } else {
        node->A->upperleft = node->upperleft;
        if (node->B != nullptr) {
            node->B->upperleft.first = node->upperleft.first + node->A->width;
            node->B->upperleft.second = node->upperleft.second;
        }
        node->C->upperleft.first = node->upperleft.first + node->A->width + bMinDimension;
        node->C->upperleft.second = node->upperleft.second;
    }

    rotateCounterClockwise(node->A);
    rotateCounterClockwise(node->B);
    rotateCounterClockwise(node->C);
}


void TripleTree::swapDimensions(Node* node) {
    if (node) {
        swap(node->width, node->height);
    }
}

int TripleTree::countLeaves(Node* node) const {
    if (!node) return 0; 
    if (!node->A && !node->B && !node->C) return 1;

    return countLeaves(node->A) + countLeaves(node->B) + countLeaves(node->C);
}

void TripleTree::clearNode(Node*& node) {
    if (!node) return;

    clearNode(node->A);
    clearNode(node->B);
    clearNode(node->C);

    delete node;
    node = nullptr;
}

Node* TripleTree::copyTree(Node* other) {
    if (!other) return nullptr;

    Node* newNode = new Node(other->upperleft, other->width, other->height);
    newNode->avg = other->avg;
    newNode->A = copyTree(other->A);
    newNode->B = copyTree(other->B);
    newNode->C = copyTree(other->C);

    return newNode;
}



