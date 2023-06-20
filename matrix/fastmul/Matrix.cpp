#include "Matrix.h"

Matrix Matrix::get_block(const std::pair<size_t, size_t> begin, const std::pair<size_t, size_t> end) const
{
	std::vector<std::vector<double>> matr(end.second - begin.second + 1, std::vector<double>(end.first - begin.first + 1));
	for (int i = begin.second; i <= end.second; i++)
	{
		for (int j = begin.first; j <= end.first; j++)
		{
			matr[i - begin.second][j - begin.first] = matrix[i][j];
		}
	}
	return Matrix(matr);
}

Matrix::Matrix(unsigned rows, unsigned cols)
{
	matrix = std::vector<std::vector<double>>(rows, std::vector<double>(cols));
}

Matrix::Matrix(const std::vector<std::vector<double>>& m)
{
	matrix = m;
}

Matrix::Matrix(std::vector<std::vector<double>>&& m)
{
	matrix = m;
}

Matrix Matrix::plus(const Matrix& arg) const
{
	Matrix result(*this);
	for (size_t i = 0; i < rows(); i++)
	{
		for (size_t j = 0; j < cols(); j++)
		{
			result.matrix[i][j] += arg.matrix[i][j];
		}
	}
	return result;
}

Matrix Matrix::minus(const Matrix& arg) const
{
	Matrix result(*this);
	for (size_t i = 0; i < rows(); i++)
	{
		for (size_t j = 0; j < cols(); j++)
		{
			result.matrix[i][j] -= arg.matrix[i][j];
		}
	}
	return result;
}

void Matrix::resize(const size_t n, const size_t m)
{
	matrix.resize(n);
	for (size_t i = 0; i < n; i++)
	{
		matrix[i].resize(m);
	}
}

Matrix Matrix::operator*(const Matrix& arg) const
{
	Matrix result((*this).rows(), arg.cols());
	for (size_t i = 0; i < rows(); i++)
	{
		for (size_t j = 0; j < arg.cols(); j++)
		{
			for (size_t k = 0; k < cols(); k++)
				result.matrix[i][j] += matrix[i][k] * arg.matrix[k][j];
		}
	}
	return result;
}

size_t Matrix::rows() const
{
	return matrix.size();
}

size_t Matrix::cols() const
{
	return matrix[0].size();
}

void Matrix::print() const
{
	for (size_t i = 0; i < rows(); i++)
	{
		for (size_t j = 0; j < cols(); j++)
		{
			std::cout << matrix[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

Matrix Matrix::_fast_mul(const Matrix& arg1, const Matrix& arg2)
{
	if (arg1.rows() <= arg1.min_fastmul_size)
		return arg1 * arg2;

	int n = arg1.rows();
	int half_n = n / 2;
	Matrix a = arg1.get_block({ 0, 0 }, { half_n - 1, half_n - 1 });
	Matrix b = arg1.get_block({ half_n, 0 }, { n - 1, half_n - 1 });
	Matrix c = arg1.get_block({ 0, half_n }, { half_n - 1, n - 1 });
	Matrix d = arg1.get_block({ half_n, half_n }, { n - 1, n - 1 });
	Matrix e = arg2.get_block({ 0, 0 }, { half_n - 1, half_n - 1 });
	Matrix g = arg2.get_block({ half_n, 0 }, { n - 1, half_n - 1 });
	Matrix f = arg2.get_block({ 0, half_n }, { half_n - 1, n - 1 });
	Matrix h = arg2.get_block({ half_n, half_n }, { n - 1, n - 1 });
	Matrix p1 = _fast_mul(a, g.minus(h));
	Matrix p2 = _fast_mul(a.plus(b), h);
	Matrix p3 = _fast_mul(c.plus(d), e);
	Matrix p4 = _fast_mul(d, f.minus(e));
	Matrix p5 = _fast_mul(a.plus(d), e.plus(h));
	Matrix p6 = _fast_mul(b.minus(d), f.plus(h));
	Matrix p7 = _fast_mul(a.minus(c), e.plus(g));

	Matrix res1 = p5.plus(p4).minus(p2).plus(p6);
	Matrix res2 = p1.plus(p2);
	Matrix res3 = p3.plus(p4);
	Matrix res4 = p5.plus(p1).minus(p3).minus(p7);

	Matrix res(n, n);

	for (size_t i = 0; i < half_n; i++)
	{
		std::copy(res1.matrix[i].begin(), res1.matrix[i].end(), res.matrix[i].begin());
		std::copy(res2.matrix[i].begin(), res2.matrix[i].end(), res.matrix[i].begin() + half_n);
	}
	for (size_t i = 0; i < half_n; i++)
	{
		std::copy(res3.matrix[i].begin(), res3.matrix[i].end(), res.matrix[i + half_n].begin());
		std::copy(res4.matrix[i].begin(), res4.matrix[i].end(), res.matrix[i + half_n].begin() + half_n);
	}
	return res;
}

Matrix fast_mul(const Matrix& arg1, const Matrix& arg2)
{
	int max_rows = std::max(arg1.rows(), arg2.rows());
	int max_cols = std::max(arg1.cols(), arg2.cols());
	int n = std::max(max_rows, max_cols);  // макс. число строк или столбцов
	double count = (double)n / arg1.min_fastmul_size;
	int all_size = 1;
	while (all_size < count)
		all_size *= 2;
	n = all_size * arg1.min_fastmul_size;

	Matrix _arg1(arg1), _arg2(arg2);
	_arg1.resize(n, n);
	_arg2.resize(n, n);

	auto res = Matrix::_fast_mul(_arg1, _arg2);

	res.resize(arg1.rows(), arg2.cols());
	return res;
}

