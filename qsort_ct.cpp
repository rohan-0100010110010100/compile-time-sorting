/*
Author :  Rohan verma (cryptecx.xyz)
Build commandline:  g++ -std=c++1z -g -v qsort_ct.cpp -o qsort_ct

 The MIT License (MIT)

Copyright © 2017 cryptecx.xyz

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <array>
#include <functional>
#include <iostream>
#include <iterator>

namespace sort_ct
{

template <typename RAIt>
constexpr RAIt next(RAIt it, typename std::iterator_traits<RAIt>::difference_type n = 1)
{
	return it + n;
}

template <typename RAIt, typename _Compare>
constexpr RAIt min_element(RAIt __first, RAIt __last, _Compare __comp)
{

	if (__first == __last)
		return __first;
	RAIt __result = __first;
	while (++__first != __last)
		if (__comp(*__first, *__result))
			__result = __first;
	return __result;
}

template <typename RAIt>
constexpr auto distance(RAIt first, RAIt last)
{
	return last - first;
}

template <class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b)
{
	auto temp = std::move(*a);
	*a = std::move(*b);
	*b = std::move(temp);
}

template <class InputIt, class UnaryPredicate>
constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q)
{
	for (; first != last; ++first)
	{
		if (!q(*first))
		{
			return first;
		}
	}
	return last;
}

template <class ForwardIt, class UnaryPredicate>
constexpr ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPredicate p)
{
	first = sort_ct::find_if_not(first, last, p);
	if (first == last)
		return first;

	for (ForwardIt i = sort_ct::next(first); i != last; ++i)
	{
		if (p(*i))
		{
			sort_ct::iter_swap(i, first);
			++first;
		}
	}
	return first;
}
}

template <class RAIt, class Compare = std::less<>>
constexpr void quick_sort(RAIt first, RAIt last, Compare cmp = Compare{})
{
	auto const N = sort_ct::distance(first, last);
	if (N <= 1)
		return;
	auto const pivot = *sort_ct::next(first, N / 2);
	auto const middle1 = sort_ct::partition(first, last, [=](auto const &elem) {
		return cmp(elem, pivot);
	});
	auto const middle2 = sort_ct::partition(middle1, last, [=](auto const &elem) {
		return !cmp(pivot, elem);
	});
	quick_sort(first, middle1, cmp); // assert(std::is_sorted(first, middle1, cmp));
	quick_sort(middle2, last, cmp);  // assert(std::is_sorted(middle2, last, cmp));
}

template <class RAIt, class Compare = std::less<>>
constexpr void selection_sort(RAIt first, RAIt last, Compare cmp = Compare{})
{
	for (auto it = first; it != last; ++it)
	{
		auto const selection = sort_ct::min_element(it, last, cmp);
		sort_ct::iter_swap(selection, it);
		//assert(std::is_sorted(first, std::next(it), cmp));
	}
}

template <typename Range>
constexpr decltype(auto) selectsort(Range &&range)
{
	selection_sort(std::begin(range), std::end(range));
	return range;
}

template <typename Range>
constexpr decltype(auto) qsort(Range &&range)
{
	quick_sort(std::begin(range), std::end(range));
	return range;
}

int main()
{
	constexpr auto arr_selectionsort = selectsort(std::array<int, 8>{4, 2, 1, 3, 8, 9, 887, 77});
	constexpr auto arr_quicksort = qsort(std::array<int, 8>{4, 2, 1, 3, 8, 9, 887, 77});
	std::cout << "Selection sort" << '\n';
	std::copy(std::cbegin(arr_selectionsort), std::cend(arr_selectionsort),
			  std::ostream_iterator<int>{std::cout, " "});
	std::cout << '\n';
	std::cout << "Quicksort" << '\n';
	std::copy(std::cbegin(arr_quicksort), std::cend(arr_quicksort),
			  std::ostream_iterator<int>{std::cout, " "});
}
