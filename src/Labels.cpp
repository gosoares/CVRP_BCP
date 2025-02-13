#include "Labels.h"

LabelBucket::LabelBucket(int64_t vertex)
    : vertex(vertex) {}

LabelBucket::~LabelBucket() {
    for (Label* label : this->labels) {
        delete label;
    }
    this->labels.clear();
}

void LabelBucket::add(double cost, boost::dynamic_bitset<> forbidden, const Label* previous) {
    Label* newLabel = new Label{vertex, cost, previous, forbidden};

    // Find insertion position and check if newLabel is dominated
    auto it = this->labels.begin();
    for (; it != this->labels.end(); ++it) {
        if ((*it)->cost > newLabel->cost) break;

        if ((*it)->forbiddenDominates(newLabel)) {
            delete newLabel;
            return;
        }
    }

    // Insert the new label at the found position
    it = this->labels.insert(it, newLabel);

    // Check if newLabel dominates any existing labels after insertion point
    ++it;
    while (it != this->labels.end()) {
        if (newLabel->forbiddenDominates(*it)) {
            delete *it;
            it = this->labels.erase(it);
        } else {
            ++it;
        }
    }
}

void LabelBucket::clear() {
    for (Label* label : this->labels) {
        delete label;
    }
    this->labels.clear();
}
