# Ternary Tree Image Manipulation & Compression System

## Overview

The Ternary Tree Image Manipulation & Compression System is a cutting-edge application developed to demonstrate the potential of ternary trees in the field of image processing. This project focuses on using a ternary (3-ary) tree to efficiently represent and manipulate images, allowing for advanced operations such as image compression, rotation, and horizontal flipping.

## Key Features

- **Ternary Tree Construction**: Builds a ternary tree representation of an image, where each node corresponds to a specific region and stores the average color.
- **Image Compression**: Utilizes the tree's structure to compress images by pruning nodes based on color tolerance, significantly reducing file size while maintaining visual quality.
- **Image Manipulation**: Supports various image manipulation techniques, including horizontal flipping and counter-clockwise rotation, through efficient tree transformations.
- **High-Fidelity Image Rendering**: Renders compressed images back to PNG format, ensuring high quality and minimal loss of detail.

## Demonstration

### Original Image
Here is the original image before any manipulation or compression:

![Original Image](https://raw.githubusercontent.com/isaacfungg/Image-Manipulation-Compression-System/main/data/kkkk-256x224-resized.png)

### Compressed Image
This is the image after applying the compression algorithm, demonstrating the system's ability to significantly reduce image size while preserving essential details:

![Compressed Image](https://raw.githubusercontent.com/isaacfungg/Image-Manipulation-Compression-System/main/data/kkkk_pruned-resized.png)

## Getting Started

To get started with the Ternary Tree Image Manipulation & Compression System, please ensure you have a compatible C++ compiler installed on your system. Follow these steps to compile and run the application:

1. Clone the repository:
   ```sh
   git clone https://github.com/isaacfungg/Image-Manipulation-Compression-System.git
