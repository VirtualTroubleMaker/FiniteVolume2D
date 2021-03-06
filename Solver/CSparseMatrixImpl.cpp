#include "CSparseMatrixImpl.h"

#include <iostream>
#include <iomanip>

#include <boost/assert.hpp>


CSparseMatrixImpl::CSparseMatrixImpl(boost::uint64_t ncols) : ncols_(ncols), finalized_(false) {}

boost::uint64_t
CSparseMatrixImpl::getRows() const {
    return 0;
}

boost::uint64_t
CSparseMatrixImpl::getCols() const {
    return 0;
}

double
CSparseMatrixImpl::operator()(boost::uint64_t i, boost::uint64_t j) const {
    // i: x, j: y

    Col_t & col = data_[i];
    return col[j];
}

double &
CSparseMatrixImpl::operator()(boost::uint64_t i, boost::uint64_t j) {
    // i: x, j: y

    if (finalized_)
        throw std::exception("CSparseMatrixImpl::operator(): Matrix already finalized");

    Col_t & col = data_[i];
    return col[j];
}

void
CSparseMatrixImpl::solve(Vec const & b, Vec & x) const {
    /* compute A x = b */
    if (!finalized_)
        throw std::exception("CSparseMatrixImpl::solve(): Matrix not yet finalized");

    bool assert_cond = b.size() == ncols_ && b.size() == x.size();
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("CSparseMatrixImpl::solve(): Out of range error");

    typedef decltype(nelements_.size()) size_type;
    size_type nrows = nelements_.size() - 1;

    // All rows
    for (int row = 0; row < nrows; ++row) {
        // Number of non-zero columns for this row
        boost::uint64_t ncol = nelements_[row + 1] - nelements_[row];
        boost::uint64_t offset = nelements_[row];

        double tmp = 0;

        // All non-zero columns
        for (int icol = 0; icol < ncol; ++icol) {
            boost::uint64_t col = columns_[offset + icol];
            double a_ij = elements_[offset + icol];

            tmp += (a_ij * b[col]);
        }

        x[row] = tmp;
    }
}

void 
CSparseMatrixImpl::finalize() const {
    // Convert to compressed row storage format
    int nelements = 0;
    int nelements_total = 0;

    Row_t::const_iterator row_it(data_.begin());
    Row_t::const_iterator row_it_end(data_.end());

    // all rows
    for (; row_it != row_it_end; ++row_it) {
//        int row = (*row_it).first;

        Col_t const & col = (*row_it).second;

        // all columns
        Col_t::const_iterator col_it(col.begin());
        Col_t::const_iterator col_it_end(col.end());

        for (; col_it != col_it_end; ++col_it) {
            boost::uint64_t col = (*col_it).first;
            double value = (*col_it).second;

            elements_.push_back(value);
            columns_.push_back(col);

            // Number of non-zero elements
            nelements++;
        }
        nelements_.push_back(nelements_total);
        nelements_total += nelements;
        nelements = 0;
    }
    nelements_.push_back(nelements_total);

    // Matrix has been finalized
    finalized_ = true;
}

void
CSparseMatrixImpl::print() const {
    // Number of rows
    typedef decltype(nelements_.size()) size_type;
    size_type nrows = nelements_.size() - 1;

    std::cout << std::endl;

    for (size_type row = 0; row < nrows; ++row) {
        // Number of non-zero columns for this row
        boost::uint64_t ncol = nelements_[row + 1] - nelements_[row];
        boost::uint64_t offset = nelements_[row];

        int icol = 0;
        for (size_type col = 0; col < nrows; ++col) {
            // leading zeros
            if (columns_[offset + icol] > col) {
                std::cout << std::setw(8) << 0 << " ";
                continue;
            }

            double a_ij = elements_[offset + icol];
            std::cout << std::setw(8) << a_ij << " ";

            ++icol;

            if (icol == ncol) {
                // trailing zeros
                size_type diff = nrows - col;

                while(--diff)
                    std::cout << std::setw(8) << 0 << " ";

                break;
            }
        }

        std::cout << std::endl;
    }
}
