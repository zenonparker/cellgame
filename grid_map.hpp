////////////////////////////////////////////////////////////////////////////////
///
/// @file grid_map.hpp
/// @brief Helper class for 2d selection operations
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_GRID_MAP_HPP
#define CELL_GRID_MAP_HPP

#include <map>
#include <vector>
#include <location.hpp>

namespace cell {

	template<class T>
	class GridMap {
		void insert(const Location &l, const T &t);
		void erase(const Location &l, const T &t);
		std::vector<T> find(const Location &l, const location &l2);
		T find(const Location &l);
	private:
		std::map<Location,T> grid_map_;
	};

	template<class T>
	void GridMap<T>::add(const Location &l, const T &t) {

	}

	template<class T>
	void GridMap<T>::remove(const Location &l, const T &t) {
	}

	template<class T>
	std::vector<T> GridMap<T>::between(const Location &l, const location &l2) {
	}


} // end namespace cell

#endif // CELL_GRID_HPP

