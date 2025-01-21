#include "Labels.h"

Labels::Labels(int64_t vertex, int64_t maxLabels)
    : vertex(vertex)
    , maxLabels(maxLabels) {};

Labels::~Labels() {
    for (Label* label : this->labels) {
        delete label;
    }
    this->labels.clear();
}

void Labels::add(double cost, const Label* previous) {
    int64_t insertIndex = this->labels.size();
    while (insertIndex > 0 && this->labels[insertIndex - 1]->cost > cost) {
        insertIndex--;
    }

    if (this->labels.size() < this->maxLabels) {  // has space to insert without removing another
        if (insertIndex == this->labels.size()) {
            this->labels.push_back(new Label{this->vertex, cost, previous});
        } else {
            this->labels.insert(this->labels.begin() + insertIndex, new Label{this->vertex, cost, previous});
        }
        return;
    }
    if (insertIndex == this->labels.size()) return;  // its not better than any label

    // shift labels to the right
    delete this->labels.back();
    for (int64_t i = this->labels.size() - 2; i >= insertIndex; i--) {
        this->labels[i + 1] = this->labels[i];
    }

    this->labels[insertIndex] = new Label{this->vertex, cost, previous};
}

const Label* Labels::getBestLabelToExtendTo(int64_t vertex) const {
    int64_t index = 0;
    while (index < this->labels.size() && this->labels[index]->previous != nullptr &&
           this->labels[index]->previous->vertex == vertex) {
        index++;
    }
    return index == this->labels.size() ? nullptr : this->labels[index];
}

void Labels::clear() {
    for (Label* label : this->labels) {
        delete label;
    }
    this->labels.clear();
}
