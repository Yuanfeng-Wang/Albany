//*****************************************************************//
//    Albany 3.0:  Copyright 2016 Sandia Corporation               //
//    This Software is released under the BSD license detailed     //
//    in the file "license.txt" in the top-level Albany directory  //
//*****************************************************************//
#ifndef MOR_GAUSSNEWTONOPERATORFACTOR_HPP
#define MOR_GAUSSNEWTONOPERATORFACTOR_HPP

#include "MOR_ReducedOperatorFactory.hpp"

class Epetra_MultiVector;
class Epetra_CrsMatrix;
class Epetra_Operator;

#include "MOR_ReducedJacobianFactory.hpp"

#include "Teuchos_RCP.hpp"

namespace MOR {

template <typename Derived>
class GaussNewtonOperatorFactoryBase : public ReducedOperatorFactory {
public:
	explicit GaussNewtonOperatorFactoryBase(const Teuchos::RCP<const Epetra_MultiVector> &reducedBasis);

	virtual bool fullJacobianRequired(bool residualRequested, bool jacobianRequested) const;

	virtual const Epetra_MultiVector &rightProjection(const Epetra_MultiVector &fullVec, Epetra_MultiVector &result) const;
	virtual const Epetra_MultiVector &leftProjection(const Epetra_MultiVector &fullVec, Epetra_MultiVector &result) const;
	virtual const Epetra_MultiVector &leftProjection_ProjectedSol(const Epetra_MultiVector &fullVec, Epetra_MultiVector &result) const;

	virtual Teuchos::RCP<Epetra_CrsMatrix> reducedJacobianNew();
	virtual const Epetra_CrsMatrix &reducedJacobianL(Epetra_CrsMatrix &result) const;
	virtual const Epetra_CrsMatrix &reducedJacobianR(Epetra_CrsMatrix &result) const;
	virtual const Epetra_CrsMatrix &reducedJacobian_ProjectedSol(Epetra_CrsMatrix &result) const;

	virtual void fullJacobianIs(const Epetra_Operator &op);

	virtual Teuchos::RCP<const Epetra_MultiVector> getPremultipliedReducedBasis() const;
	virtual Teuchos::RCP<const Epetra_MultiVector> getReducedBasis() const;
	virtual Teuchos::RCP<const Epetra_MultiVector> getLeftBasisCopy() const;
	virtual Teuchos::RCP<const Epetra_MultiVector> getRightBasis() const;

	virtual Teuchos::RCP<const Epetra_MultiVector> getScaling() const;
	virtual void setScaling(Epetra_CrsMatrix &jacobian) const;
	virtual void applyScaling(const Epetra_MultiVector &vector) const;

	virtual Teuchos::RCP<const Epetra_MultiVector> getPreconditioner() const;
	virtual void setPreconditionerDirectly(Epetra_MultiVector &vector) const;
	virtual void setPreconditioner(Epetra_CrsMatrix &jacobian) const;
	virtual void applyPreconditioner(const Epetra_MultiVector &vector) const;
	virtual void applyPreconditionerTwice(const Epetra_MultiVector &vector) const;

	virtual Teuchos::RCP<Ifpack_Preconditioner> getPreconditionerIfpack() const;
	virtual void setPreconditionerIfpack(Epetra_CrsMatrix &jacobian, std::string ifpackType) const;
	virtual void applyPreconditionerIfpack(const Epetra_MultiVector &vector) const;
	virtual void applyPreconditionerIfpackTwice(const Epetra_MultiVector &vector) const;

	virtual Teuchos::RCP<const Epetra_CrsMatrix> getJacobian() const;
	virtual void setJacobian(Epetra_CrsMatrix &jacobian) const;
	virtual void applyJacobian(const Epetra_MultiVector &vector) const;

protected:

private:
	Teuchos::RCP<const Epetra_MultiVector> reducedBasis_;

	ReducedJacobianFactory jacobianFactory_;

	Teuchos::RCP<Epetra_MultiVector> scaling_;
	Teuchos::RCP<Epetra_MultiVector> preconditioner_;
	Teuchos::RCP<Epetra_MultiVector> leftbasis_;
	mutable Teuchos::RCP<Ifpack_Preconditioner> preconditioner_ifpack_;
	mutable Teuchos::RCP<Epetra_CrsMatrix> jacobian_;

	Teuchos::RCP<const Epetra_MultiVector> getLeftBasis() const;
};

class GaussNewtonOperatorFactory : public GaussNewtonOperatorFactoryBase<GaussNewtonOperatorFactory> {
public:
	explicit GaussNewtonOperatorFactory(const Teuchos::RCP<const Epetra_MultiVector> &reducedBasis);

	Teuchos::RCP<const Epetra_MultiVector> leftProjectorBasis() const;
};

class GaussNewtonMetricOperatorFactory : public GaussNewtonOperatorFactoryBase<GaussNewtonMetricOperatorFactory> {
public:
	GaussNewtonMetricOperatorFactory(const Teuchos::RCP<const Epetra_MultiVector> &reducedBasis,
			const Teuchos::RCP<const Epetra_Operator> &metric);

	// Overridden
	virtual void fullJacobianIs(const Epetra_Operator &op);

	Teuchos::RCP<const Epetra_MultiVector> leftProjectorBasis() const;

private:
	Teuchos::RCP<const Epetra_Operator> metric_;

	Teuchos::RCP<Epetra_MultiVector> premultipliedLeftProjector_;

	void updatePremultipliedLeftProjector();
};

} // namespace MOR

#endif /* MOR_GAUSSNEWTONOPERATORFACTOR_HPP */
