# Checkers

## Overview

This is a simple implementation of a game of checkers with a non-human opponent implemented
using an adversarial search algorithm.

## Description

The implementation is written in C++ using
the SFML library as backbone for rendering.

## Implementation

The algorithm implemented for finding the optimal move is Minimax Search with Alpha-Beta optimization.

There are several rules for evaluation encoded, as well as, a prioritization of moves that I added, this way, creating a personality for the AI opponent as a play style(i.e. more aggresive or passive) and eliminating undesired moves.

The graphical elements are not perfect, they are there only as a Proof-of-Concept.