#ifndef SRC_LIB_POINTERVECTOR_H_
#define SRC_LIB_POINTERVECTOR_H_

#include <vector>
#include <functional>
#include <memory>

#include "JSON_Serializable.h"

/**
 * This is the base class for any vector that holds pointers to data we keep.
 * On destruction or a call to erase, we destroy not only our contents, but the
 * pointed-to values. If you pull something from us, the person pulling it owns it.
 *
 * It is possible to abuse this object by doing things like calling clear() directly.
 * Please don't.
 */
template <class ObjectType>
class PointerVector : public std::vector<std::shared_ptr<ObjectType>> {
public:
    typedef std::function<bool(ObjectType *)> Comparator;

    /** Constructor. */
    PointerVector() = default;

    /**
     * Copy constructor. Note that we set iOwnTheData to false, which means if you
     * retain this object longer than the original, you're going to have trouble.
     */
    PointerVector(const PointerVector<ObjectType> &orig) {
        *this = orig;
    }

    /**
     * Move constructor.
     */
    PointerVector(PointerVector<ObjectType> &&orig) {
        this->swap(orig);
    }

	/**
	 * Destructor does an eraseAll().
	 */
	virtual ~PointerVector() { eraseAll(); }

    /**
     * Copy operator.
     */
    PointerVector<ObjectType> & operator=(const PointerVector<ObjectType> &orig) {
        if (this != &orig) {
            eraseAll();

            for (std::shared_ptr<ObjectType> ptr: orig) {
                this->push_back(ptr);
            }
        }
        return *this;
    }

    /**
     * Move operator.
     */
    PointerVector<ObjectType> & operator=(PointerVector<ObjectType> &&orig) noexcept {
        std::cout << "Move operator." << std::endl;
        if (this != &orig) {
            this->swap(orig);
        }
        return *this;
    }

	/**
	 * Do we have this object?
	 */
    bool contains(const std::shared_ptr<ObjectType> ptr) {
		for (auto iter = this->begin(); iter != this->end(); ++iter) {
			if (*iter == ptr) {
				return true;
			}
		}
		return false;
	}

    /**
     * What is the index of this pointer?
     *
     * @returns Index of this pointer or -1 if not found.
     */
	int indexOf(ObjectType *ptr) {
		int retVal = 0;
		for (auto iter = this->begin(); iter != this->end(); ++iter) {
			if (*iter == ptr) {
				return retVal;
			}
			++retVal;
		}
		return -1;
	}

	/**
	 * This is an attempt to catch people who call clear() instead of eraseAll(),
	 * but if they're passing us around as a superclass, they can cheat, as the STL
	 * people, in their wisdom, don't make their methods virtual.
	 */
	void clear() {
		eraseAll();
	}

	/**
	 * Erase our contents, freeing any memory we're pointed to.
	 */
	void eraseAll() {
        std::vector<std::shared_ptr<ObjectType>>::clear();
	}

	/**
	 * This removes this object without deleting the object. The caller now owns the data.
	 */
    void remove(std::shared_ptr<ObjectType> ptr) {
		for (auto iter = this->begin(); iter != this->end(); ++iter) {
			if (*iter == ptr) {
				this->erase(iter);
				break;
			}
		}
	}

    ObjectType * findWith(Comparator funct) {
        for (ObjectType *ptr: *this) {
            if (funct(ptr)) {
                return ptr;
            }
        }
        return nullptr;
    }

    /**
     * perform a binary search. the compare function should return < 0 if we need
     * to move closer to the front == 0 for a matchin, and > 0 to move further ahead.
     */
    int binarySearchForIndex(std::function<bool(ObjectType *)> compare, bool returnInsertLoc = false, int low = 0, int high = 0) const {
        if (high == 0) {
            high = this->size();
        }
        if (low >= high) {
            return returnInsertLoc ? low : -1;
        }
        int mid = (low + high) / 2;
        ObjectType *ptr = this->at(mid);
        int rv = compare(ptr);

        if (rv < 0) {
            return binarySearchForIndex(compare, returnInsertLoc, low, mid);
        }
        if (rv > 0) {
            return binarySearchForIndex(compare, returnInsertLoc, mid + 1, high);
        }

        return mid;
    }

    friend std::ostream& operator<<(std::ostream& out, const PointerVector<ObjectType> & vector) {
        for (ObjectType *ptr: vector) {
            out << *ptr << std::endl;
        }
        return out;
    }
};

template <class ObjectType>
class JSON_Serializable_PointerVector : public PointerVector<ObjectType>, public JSON_Serializable {
public:
    JSON_Serializable_PointerVector() { }
    JSON_Serializable_PointerVector(bool v): PointerVector<ObjectType>(v) { }

    void fromJSON(const nlohmann::json & array) {
        for (auto iter = array.begin(); iter != array.end(); ++iter) {
            nlohmann::json obj = *iter;

            ObjectType * thisDiff = new ObjectType();
            thisDiff->fromJSON(obj);
            this->push_back(thisDiff);
        }
    }

    void toJSON(nlohmann::json & array) const {
        for (ObjectType * obj: *this) {
            nlohmann::json childJson = nlohmann::json::object();
            obj->toJSON(childJson);
            array.push_back(childJson);
        }
    }
};

#endif /* SRC_LIB_POINTERVECTOR_H_ */
