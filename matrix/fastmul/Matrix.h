#pragma once

#include <vector>
#include <iostream>

class Matrix
{
	std::vector<std::vector<double>> matrix;
	static const size_t min_fastmul_size = 256; // ?
	static Matrix _fast_mul(const Matrix& arg1, const Matrix& arg2); //рекурсивная подфункция fastmul
public:
	Matrix(unsigned rows, unsigned cols); //конструктор 0 матрицы
	Matrix(const std::vector<std::vector<double>>& m); 
	Matrix(std::vector<std::vector<double>>&& m);

	Matrix operator* (const Matrix& arg) const;
	size_t rows() const; //строки
	size_t cols() const; //столбцы
	void print() const;
	Matrix get_block(const std::pair<size_t, size_t> begin, const std::pair<size_t, size_t> end) const; 
	friend Matrix fast_mul(const Matrix& arg1, const Matrix& arg2); //основная функция fastmul

private:
	Matrix plus(const Matrix& arg) const;
	Matrix minus(const Matrix& arg) const;
	void resize(const size_t n, const size_t m);
};

