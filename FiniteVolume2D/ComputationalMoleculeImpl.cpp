#include "ComputationalMoleculeImpl.h"

#include "ComputationalVariableHolder.h"
#include "ComputationalCell.h"


ComputationalMoleculeImpl::ComputationalMoleculeImpl(std::string const & name, ComputationalVariableHolder::Ptr const & cvar_holder)
    :
    name_(name), cvar_holder_(cvar_holder) {}

void
ComputationalMoleculeImpl::print() const {
    // print computational molecule

    std::cout << std::endl << "Molecule name: " << name() << std::endl;

    std::for_each(data_.begin(), data_.end(), [this](ComputationalMolecule_t::value_type const & item) {
        // item.first  : ComputationalVariable::Id_t
        // item.second : weight
        // find this contribution in "in"
        ComputationalVariable::Id_t const & cvar_id = item.first;
        double const & weight = item.second;

        ComputationalVariable::Ptr cvar = cvar_holder_->get(cvar_id);
        ComputationalCell::Ptr const & ccell = cvar->getCell();

        std::cout << "(Cell id, var name, weight) = (" << ccell->id() << ", " << cvar->getName() << ", " << weight << ")" << std::endl;
    });

    std::cout << "Source term: " << source_term_.value() << std::endl;
}

std::string const &
ComputationalMoleculeImpl::name() const {
    return name_;
}

void
ComputationalMoleculeImpl::add(ComputationalVariable const & cvar, double weight) {
    data_[cvar.id()] += weight;
}

boost::optional<double>
ComputationalMoleculeImpl::getWeight(ComputationalVariable const & cvar) const {
    auto it = data_.find(cvar.id());
    if (it == data_.end())
        return boost::optional<double>();
    return it->second;
}

ComputationalMoleculeImpl::size_type
ComputationalMoleculeImpl::size() const {
    return data_.size();
}

bool
ComputationalMoleculeImpl::empty() const {
    return data_.empty();
}

SourceTerm &
ComputationalMoleculeImpl::getSourceTerm() {
    return source_term_;
}

SourceTerm const &
ComputationalMoleculeImpl::getSourceTerm() const {
    return source_term_;
}

bool
ComputationalMoleculeImpl::addMolecule(ComputationalMoleculeImpl & in) const {
    std::for_each(data_.begin(), data_.end(), [&in](ComputationalMolecule_t::value_type const & item) {
        // item.first  : ComputationalVariable::Id_t
        // item.second : weight
        // find this contribution in "in"
        ComputationalVariable::Id_t const & cvar_id = item.first;
        double const & weight = item.second;

        auto it = in.data_.find(cvar_id);
        if (it == in.data_.end())
            in.data_[cvar_id] = weight;

        else
            in.data_[cvar_id] += weight;
    });

    // account for source term
    in.getSourceTerm() += getSourceTerm().value();

    return true;
}
