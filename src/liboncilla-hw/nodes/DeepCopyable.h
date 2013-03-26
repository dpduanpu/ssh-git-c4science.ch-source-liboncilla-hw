#pragma once

#include <set>

namespace liboncilla {
namespace hw {

/**
 * This is an internal interface to make resource nodes deep-copyable.
 *
 * @todo This interface should be removed, once robo-xeno is upgraded accordingly.
 */
class DeepCopyable {
public:
    DeepCopyable();
    virtual ~DeepCopyable();

    /**
     * Tell the node to deep-copy its resources. This one has to be implemented
     * by the ResourceNode implementing this interface. It has to make a deep
     * copy of all contained resources (JointAngles, Forces, etc., usually
     * shared pointers).
     */
    virtual void deepCopyResources() = 0;

    /**
     * Mark this node as changed by adding it to ChangedDeepCopyables.
     */
    void mark();
};

/**
 * Set of DeepCopyable items, that are to be deep copied.
 */
static std::set<DeepCopyable*> ChangedDeepCopyables;

/**
 * Make deep copies of all items in the ChangedDeepCopyables set.
 */
static void makeDeepCopies();

/**
 * Clear the list of to be deep copied objects, ChangedDeepCopyables.
 */
static void clearMarks();

}
}
