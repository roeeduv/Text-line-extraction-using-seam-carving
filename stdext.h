#ifndef _STD_EXT_H_ 
#define _STD_EXT_H_

#include <opencv/cv.h>
#include <vector>

namespace stdext {
	
	template<class T>
	void  copy(std::vector<T>& src, std::vector<T>& dst){
		dst.clear();
		dst.insert(dst.begin(), src.begin(), src.end());
	}

	template<class T>
	void  subtract(std::vector<T>& list, std::vector<T>& to_subtract){
		std::vector<T>::iterator iter, jter;
		for (iter = to_subtract.begin(); iter != to_subtract.end(); iter++){
			for (jter = list.begin(); jter != list.end(); jter++){
				if (*jter == *iter){
					list.erase(jter);
					break;
				}
			}
		}
	}
}

#endif 