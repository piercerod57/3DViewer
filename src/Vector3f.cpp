/**
 * CSC 4565/5565 Introduction to Computer Graphics
 * Instructor: Dr. Min Choi
 * Name: Shane Transue
 * ID: 101342320
 * Due: October 17, 2011
 *
 * Homework 3 (Icosahedron and Lighting) - Write a program that allows interactive
 * subdivision and rendering of an icosahedron.
 */

#include <math.h>
#include "Vector3f.h"

Vector3f::Vector3f() {
	for ( int i = 0; i < 3; i++ )
		this->data[i] = 0.0f;
}

Vector3f::Vector3f(float x, float y, float z) {
	this->data[0] = x;
	this->data[1] = y;
	this->data[2] = z;
}

float Vector3f::dot(const Vector3f& a, const Vector3f& b) {
	return a.data[0] * b.data[0] + a.data[1] * b.data[1] + a.data[2] * b.data[2];
}

float Vector3f::length(const Vector3f& vector) {
	return sqrt(pow(vector.data[0], 2) + pow(vector.data[1], 2) + pow(vector.data[2], 2));
}

Vector3f Vector3f::normalize(const Vector3f& vector) {
	Vector3f result;
	float len = Vector3f::length(vector);
	result.data[0] = vector.data[0] / len;
	result.data[1] = vector.data[1] / len;
	result.data[2] = vector.data[2] / len;
	return result;
}

Vector3f Vector3f::cross(const Vector3f& a, const Vector3f& b) {
	Vector3f result;
	result.data[0] = a.data[1] * b.data[2] - a.data[2] * b.data[1];
	result.data[1] = a.data[2] * b.data[0] - a.data[0] * b.data[2];
	result.data[2] = a.data[0] * b.data[1] - a.data[1] * b.data[0];
	return result;
}

Vector3f Vector3f::operator + (const Vector3f& vector) {
	Vector3f result;
	result.data[0] = this->data[0] + vector.data[0];
	result.data[1] = this->data[1] + vector.data[1];
	result.data[2] = this->data[2] + vector.data[2];
	return result;
}

Vector3f Vector3f::operator - (const Vector3f& vector) {
	Vector3f result;
	result.data[0] = this->data[0] - vector.data[0];
	result.data[1] = this->data[1] - vector.data[1];
	result.data[2] = this->data[2] - vector.data[2];
	return result;
}

Vector3f Vector3f::operator / (const float& scalar) {
	Vector3f result;
	result.data[0] = this->data[0] / scalar;
	result.data[1] = this->data[1] / scalar;
	result.data[2] = this->data[2] / scalar;
	return result;
}

Vector3f Vector3f::operator * (const float& scalar) {
	Vector3f result;
	result.data[0] = this->data[0] * scalar;
	result.data[1] = this->data[1] * scalar;
	result.data[2] = this->data[2] * scalar;
	return result;
}