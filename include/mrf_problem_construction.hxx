#ifndef LP_MP_MRF_PROBLEM_CONSTRUCTION_HXX
#define LP_MP_MRF_PROBLEM_CONSTRUCTION_HXX

#include "solver.hxx"
#include "cycle_inequalities.hxx"
#include "tree_decomposition.hxx"
#include "arboricity.h"
#include "mrf_input.h"

#include <string>
#include <functional>

namespace LP_MP {

// expects simplex factor as unary and pairwise factors and marg message such that unary factor is on the left side and pairwise factor is on the right side
// possibly use static inheritance instead of virtual functions
template<class FACTOR_MESSAGE_CONNECTION, INDEX UNARY_FACTOR_NO, INDEX PAIRWISE_FACTOR_NO, INDEX LEFT_MESSAGE_NO, INDEX RIGHT_MESSAGE_NO>
class mrf_constructor {
public:
   using FMC = FACTOR_MESSAGE_CONNECTION;

   using UnaryFactorContainer = meta::at_c<typename FMC::FactorList, UNARY_FACTOR_NO>;
   using UnaryFactorType = typename UnaryFactorContainer::FactorType;
   using PairwiseFactorContainer = meta::at_c<typename FMC::FactorList, PAIRWISE_FACTOR_NO>;
   using PairwiseFactorType = typename PairwiseFactorContainer::FactorType;
   using LeftMessageContainer = typename meta::at_c<typename FMC::MessageList, LEFT_MESSAGE_NO>::MessageContainerType;
   using LeftMessageType = typename LeftMessageContainer::MessageType;
   using RightMessageContainer = typename meta::at_c<typename FMC::MessageList, RIGHT_MESSAGE_NO>::MessageContainerType;
   using RightMessageType = typename RightMessageContainer::MessageType;


   template<typename SOLVER>
   mrf_constructor(SOLVER& solver) : lp_(&solver.GetLP()) {}

   mrf_constructor(LP<FMC>* lp) : lp_(lp) {}

   template<typename T>
   UnaryFactorContainer* add_unary_factor(const std::initializer_list<T> c)
   {
       return add_unary_factor(c.begin(), c.end());
   }

   template<typename ITERATOR>
   UnaryFactorContainer* add_unary_factor(ITERATOR cost_begin, ITERATOR cost_end)
   {
      auto* u = lp_->template add_factor<UnaryFactorContainer>(cost_begin, cost_end);
      unaryFactor_.push_back(u);
      return u; 
    
    }

   template<typename VECTOR>
   UnaryFactorContainer* add_unary_factor(const VECTOR& cost)
   {
       return add_unary_factor(cost.begin(), cost.end());
   }

   template<typename ITERATOR>
   UnaryFactorContainer* add_unary_factor_without_relation(ITERATOR cost_begin, ITERATOR cost_end)
   {
      auto* u = lp_->template add_factor<UnaryFactorContainer>(cost_begin, cost_end);
      unaryFactor_.push_back(u);

      return u; 
   }


   void add_unary_unary_relation(std::size_t unary1, std::size_t unary2)
   {
       assert(fmax(unary1, unary2) <= unaryFactor_.size() - 1);
       lp_->AddFactorRelation(unaryFactor_[unary1], unaryFactor_[unary2]);
   }
   
   // unary factor was created elsewhere, let mrf know it
   void RegisterUnaryFactor(const INDEX node_number, UnaryFactorContainer* u)
   {
       assert(false);
      if(node_number >= unaryFactor_.size()) {
         unaryFactor_.resize(node_number+1,nullptr);
      } else {
         if(unaryFactor_[node_number] != nullptr) { throw std::runtime_error("unary factor " + std::to_string(node_number) + " already present"); }
      }
      unaryFactor_[node_number] = u;
   }

   template<typename MATRIX>
   PairwiseFactorContainer* add_pairwise_factor(const std::size_t var1, const std::size_t var2, const MATRIX& cost)
   { 
      assert(var1<var2);
      assert(!has_pairwise_factor(var1,var2));
      auto* p = lp_->template add_factor<PairwiseFactorContainer>(get_number_of_labels(var1), get_number_of_labels(var2));
      for(std::size_t i=0; i<get_number_of_labels(var1); ++i) {
          for(std::size_t j=0; j<get_number_of_labels(var2); ++j) {
              p->GetFactor()->cost(i,j) = cost(i,j);
          }
      }
      pairwiseFactor_.push_back(p);
      pairwiseIndices_.push_back(std::array<INDEX,2>({var1,var2}));
      const INDEX factorId = pairwiseFactor_.size()-1;
      pairwiseMap_.insert(std::make_pair(std::make_tuple(var1,var2), factorId));

      lp_->template add_message<LeftMessageContainer>(this->get_unary_factor(var1), p);
      lp_->template add_message<RightMessageContainer>(this->get_unary_factor(var2), p);

      return p;
   }

   template<typename MATRIX>
   PairwiseFactorContainer* add_pairwise_factor_without_relation(const std::size_t var1, const std::size_t var2, const MATRIX& cost)
   { 
      assert(var1<var2);
      assert(!has_pairwise_factor(var1,var2));
      auto* p = lp_->template add_factor<PairwiseFactorContainer>(get_number_of_labels(var1), get_number_of_labels(var2));
      for(std::size_t i=0; i<get_number_of_labels(var1); ++i) {
          for(std::size_t j=0; j<get_number_of_labels(var2); ++j) {
              p->GetFactor()->cost(i,j) = cost(i,j);
          }
      }
      pairwiseFactor_.push_back(p);
      pairwiseIndices_.push_back(std::array<INDEX,2>({var1,var2}));
      const INDEX factorId = pairwiseFactor_.size()-1;
      pairwiseMap_.insert(std::make_pair(std::make_tuple(var1,var2), factorId));

      lp_->template add_message<LeftMessageContainer>(this->get_unary_factor(var1), p);
      lp_->template add_message<RightMessageContainer>(this->get_unary_factor(var2), p);

      return p;
   }

   void add_unary_pairwise_relation(const std::size_t var1, const std::size_t var2)
   {
      auto* p = get_pairwise_factor(fmin(var1, var2), fmax(var1, var2));
      lp_->AddFactorRelation(unaryFactor_[var1], p);
      lp_->AddFactorRelation(p, unaryFactor_[var2]);
   }

   UnaryFactorContainer* get_unary_factor(const INDEX i) const { assert(i<unaryFactor_.size()); return unaryFactor_[i]; }
   PairwiseFactorContainer* get_pairwise_factor(const INDEX i) const { assert(i<pairwiseFactor_.size()); return pairwiseFactor_[i]; }
   PairwiseFactorContainer* get_pairwise_factor(const INDEX i, const INDEX j) const { 
      assert(i<j);    
      assert(j<unaryFactor_.size());
      const INDEX factor_id = get_pairwise_factor_id(i,j);
      return pairwiseFactor_[factor_id]; 
   }

   LeftMessageContainer* get_left_message(const INDEX i, const INDEX j) const
   {
      assert(i < j);
      auto* f = get_pairwise_factor(i,j);
      auto msgs = f->template get_messages<LeftMessageContainer>();
      assert(msgs.size() == 1);
      return msgs[0];
   }

   RightMessageContainer* get_right_message(const INDEX i, const INDEX j) const
   {
      assert(i < j);
      auto* f = get_pairwise_factor(i,j);
      auto msgs = f->template get_messages<RightMessageContainer>();
      assert(msgs.size() == 1);
      return msgs[0];
   }

   INDEX get_number_of_variables() const 
   { 
      assert(!(unaryFactor_.size() == 0 && pairwiseFactor_.size() > 0)); 
      return unaryFactor_.size(); 
   } // note: this is not a good idea, if unaryFactors are populated elsewhere: take maximum in pairwise factor indices then.
   INDEX get_pairwise_factor_id(const INDEX var1, const INDEX var2) const 
   {
      assert(var1<var2);
      assert(pairwiseMap_.find(std::make_tuple(var1,var2)) != pairwiseMap_.end());
      return pairwiseMap_.find(std::make_tuple(var1,var2))->second; 
   }
   bool has_pairwise_factor(const INDEX var1, const INDEX var2) const
   {
      assert(var1<var2);
      if(pairwiseMap_.find(std::make_tuple(var1,var2)) != pairwiseMap_.end()) { 
         return true;
      } else {
         return false;
      }
   }
   INDEX get_number_of_pairwise_factors() const { return pairwiseFactor_.size(); }
   std::array<INDEX,2> get_pairwise_variables(const INDEX factorNo) const { assert(factorNo < pairwiseIndices_.size()); return pairwiseIndices_[factorNo]; }
   INDEX get_number_of_labels(const INDEX i) const { assert(i < unaryFactor_.size()); return unaryFactor_[i]->GetFactor()->size(); }
   REAL GetPairwiseValue(const INDEX factorId, const INDEX i1, const INDEX i2) const
   {
      assert(i1 < get_number_of_labels( get_pairwise_variables(factorId)[0] ));
      assert(i2 < get_number_of_labels( get_pairwise_variables(factorId)[1] ));
      return (*pairwiseFactor_[factorId]->GetFactor())(i1,i2);
   }


   template<typename SOLVER>
   void Construct(SOLVER& pd) 
   {
      std::cout << "Construct MRF problem with " << unaryFactor_.size() << " unary factors and " << pairwiseFactor_.size() << " pairwise factors\n";

      // add order relations. These are important for the anisotropic weights computation to work.
      if(unaryFactor_.size() > 1) {
         for(auto it=unaryFactor_.begin(); it+1 != unaryFactor_.end(); ++it) {
            assert(*it != nullptr);
            lp_->AddFactorRelation(*it,*(it+1));
         }
      }

      for(std::size_t p=0; p<get_number_of_pairwise_factors(); ++p) {
          const auto [i,j] = get_pairwise_variables(p);
          assert(i<j);
          auto* f_i = get_unary_factor(i);
          auto* f_j = get_unary_factor(j);
          auto* f_p = get_pairwise_factor(p);
          lp_->AddFactorRelation(f_i, f_p);
          lp_->AddFactorRelation(f_p, f_j);
      }

      assert(pairwiseIndices_.size() == pairwiseFactor_.size());
   }

   template<typename STREAM>
   void WritePrimal(STREAM& s) const 
   {
      if(unaryFactor_.size() > 0) {
         for(INDEX i=0; i<unaryFactor_.size()-1; ++i) {
            s << unaryFactor_[i]->GetFactor()->primal() << ", ";
         }
         auto* f = unaryFactor_.back();
         s << f->GetFactor()->primal() << "\n";
      }
   }

  // build tree of unary and pairwise factors
  factor_tree<FMC> add_tree(std::vector<PairwiseFactorContainer*> p)
  {
     factor_tree<FMC> t;
     assert(p.size() > 0);

     // extract messages joining unaries and pairwise
     std::set<UnaryFactorContainer*> u_set; // u_set is not strictly needed, but helps in checking correctness of algorithm
     std::set<PairwiseFactorContainer*> p_set;
     UnaryFactorContainer* root = nullptr;
     for(auto* f : p) {
        p_set.insert(f); 

        auto left_msgs = f->template get_messages<LeftMessageContainer>();
        assert(left_msgs.size() == 1);
        UnaryFactorContainer* left = (*left_msgs.begin())->GetLeftFactor();
        u_set.insert(left);

        auto right_msgs = f->template get_messages<RightMessageContainer>();
        assert(right_msgs.size() == 1);
        UnaryFactorContainer* right = (*right_msgs.begin())->GetLeftFactor(); 
        u_set.insert(right);

        assert(left != right);

        root = left;
     }

     // assume some arbitrary root. build tree recursively from root
     assert(u_set.size() == p_set.size() + 1);

     std::deque<UnaryFactorContainer*> u_stack;
     u_stack.push_back(root);
     std::vector<std::tuple<Chirality, PairwiseFactorContainer*>> ordered_msgs;
     ordered_msgs.reserve(2*p.size());

     while(!u_stack.empty()) {
        auto* f = u_stack.front();
        u_stack.pop_front();
        u_set.erase(f);

        {
           auto msgs = f->template get_messages<LeftMessageContainer>();
           for(auto it=msgs.begin(); it!= msgs.end(); ++it) {
              auto* p_cand = (*it)->GetRightFactor();
              if(p_set.find(p_cand) != p_set.end()) {
                 p_set.erase(p_cand);
                 ordered_msgs.push_back(std::make_tuple(Chirality::left, p_cand));
                 // search for the other unary connected to p_cand
                 auto msgs_other = p_cand->template get_messages<RightMessageContainer>();
                 assert(msgs_other.size() == 1);
                 auto* u_other = msgs_other[0]->GetLeftFactor();
                 assert(u_set.find(u_other) != u_set.end());
                 //t.AddMessage(*(msgs_other.begin()), Chirality::right);
                 u_stack.push_back(u_other);
              }
           }
        }

        {
           auto msgs = f->template get_messages<RightMessageContainer>();
           for(auto it=msgs.begin(); it!= msgs.end(); ++it) {
              auto* p_cand = (*it)->GetRightFactor();
              if(p_set.find(p_cand) != p_set.end()) {
                 p_set.erase(p_cand);
                 //t.AddMessage((*it), Chirality::left); // or right?
                 ordered_msgs.push_back(std::make_tuple(Chirality::right, p_cand));
                 // search for the other unary connected to p_cand
                 auto msgs_other = p_cand->template get_messages<LeftMessageContainer>();
                 assert(msgs_other.size() == 1);
                 auto* u_other = msgs_other[0]->GetLeftFactor();
                 assert(u_set.find(u_other) != u_set.end());
                 //t.AddMessage(*(msgs_other.begin()), Chirality::right);
                 u_stack.push_back(u_other);
              }
           }
        }
     }

     assert(p_set.empty());

     std::reverse(ordered_msgs.begin(), ordered_msgs.end());
     for(auto e : ordered_msgs) {
         auto* p = std::get<1>(e);
         auto left_msgs = p->template get_messages<LeftMessageContainer>();
         assert(left_msgs.size() == 1);
         auto right_msgs = p->template get_messages<RightMessageContainer>();
         assert(right_msgs.size() == 1);

         if(std::get<0>(e) == Chirality::left) {
             t.add_message(right_msgs[0], Chirality::right);
             t.add_message(left_msgs[0], Chirality::left);
         } else {
             assert(std::get<0>(e) == Chirality::right);
             t.add_message(left_msgs[0], Chirality::right);
             t.add_message(right_msgs[0], Chirality::left);
         }
     }

     return t;
  }

  // compute forest cover of MRF and add each resulting tree

  auto compute_forest_cover() { return compute_forest_cover(pairwiseIndices_); }

  std::vector<factor_tree<FMC>> compute_forest_cover(const std::vector<std::array<INDEX,2>>& pairwiseIndices)
  {
     UndirectedGraph g(unaryFactor_.size(), pairwiseFactor_.size());
     for(auto e : pairwiseIndices) {
        const auto i = e[0];
        const auto j = e[1];
        g.AddEdge(i,j,1);
     }
     
     const INDEX forest_num = g.Solve();
     std::cout << "decomposed mrf into " << forest_num << " trees\n";
     std::vector<factor_tree<FMC>> trees;

     std::vector<int> parents(unaryFactor_.size());
     for(INDEX k=0; k<forest_num; ++k) {
        g.GetForestParents(k, parents.data()); // possible GetForestEdges will return pairwise ids, hence get_pairwise_factor(id) then can be called, which is faster

        union_find uf(unaryFactor_.size());
        for(INDEX i=0; i<parents.size(); ++i) {
           if(parents[i] != -1) {
              uf.merge(i, parents[i]);
           }
        }
        auto contiguous_ids = uf.get_contiguous_ids();
        
        const INDEX no_trees = uf.count();
        std::vector<std::vector<PairwiseFactorContainer*>> pairwise(no_trees);

        for(INDEX i=0; i<parents.size(); ++i) {
           if(parents[i] != -1) {
              const INDEX tree_id = contiguous_ids[uf.find(i)];
              const INDEX j = parents[i];
              assert(tree_id == contiguous_ids[uf.find(j)]);
              PairwiseFactorContainer* p = get_pairwise_factor(std::min(i,j), std::max(i,j));
              pairwise[tree_id].push_back(p);
           } 
        }
        for(INDEX t=0; t<no_trees; ++t) {
           if(pairwise[t].size() > 0) {
              trees.push_back(add_tree(pairwise[t])); 
           }
        }
     }

     auto check_pairwise_factors_present = [&trees]() -> INDEX {
        INDEX pairwise_factors_in_trees = 0;
        for(auto& tree : trees) {
           for(auto* f : tree.factors_) {
              if(dynamic_cast<PairwiseFactorContainer*>(f)) {
                 pairwise_factors_in_trees++;
              }
           }
        }
        return pairwise_factors_in_trees;
     };

     //assert(check_pairwise_factors_present() == pairwiseIndices.size());

     return std::move(trees);
  }

  void construct(const mrf_input& input)
  {
      // first input the unaries, as pairwise potentials need them to be able to link to them
      for(std::size_t i=0; i<input.no_variables(); ++i) {
          this->add_unary_factor(input.unaries[i]);
      }

      assert(input.pairwise_indices.size() == input.pairwise_values.dim1());
      for(std::size_t i=0; i<input.pairwise_indices.size(); ++i) {
          const auto var1 = input.pairwise_indices[i][0];
          const auto var2 = input.pairwise_indices[i][1];
          auto pairwise_cost = input.pairwise_values[i];
          this->add_pairwise_factor(var1,var2, pairwise_cost);
      }
  }

  LP<FMC>* get_lp() const { return lp_; }

protected:
   std::vector<UnaryFactorContainer*> unaryFactor_;
   std::vector<PairwiseFactorContainer*> pairwiseFactor_;
   
   std::vector<std::array<INDEX,2>> pairwiseIndices_;

   std::map<std::tuple<INDEX,INDEX>, INDEX> pairwiseMap_; // given two sorted indices, return factorId belonging to that index.

   //INDEX unaryFactorIndexBegin_, unaryFactorIndexEnd_; // do zrobienia: not needed anymore

   LP<FMC>* lp_;
};

// derives from a given mrf problem constructor and adds tightening capabilities on top of it, as implemented in cycle_inequalities and proposed by David Sontag
template<class MRF_PROBLEM_CONSTRUCTOR,
   INDEX TERNARY_FACTOR_NO, INDEX PAIRWISE_TRIPLET_MESSAGE12_NO, INDEX PAIRWISE_TRIPLET_MESSAGE13_NO, INDEX PAIRWISE_TRIPLET_MESSAGE23_NO> // the last indices indicate triplet factor and possible messages
class tightening_mrf_constructor : public MRF_PROBLEM_CONSTRUCTOR
{
protected:
   using MRFPC = MRF_PROBLEM_CONSTRUCTOR;
   using FMC = typename MRFPC::FMC;
   using MrfConstructorType = tightening_mrf_constructor<MRFPC, TERNARY_FACTOR_NO, PAIRWISE_TRIPLET_MESSAGE12_NO, PAIRWISE_TRIPLET_MESSAGE13_NO, PAIRWISE_TRIPLET_MESSAGE23_NO>;

   using TripletFactorContainer = meta::at_c<typename FMC::FactorList, TERNARY_FACTOR_NO>; 
   using TripletFactor = typename TripletFactorContainer::FactorType;
   using PairwiseTripletMessage12Container = typename meta::at_c<typename FMC::MessageList, PAIRWISE_TRIPLET_MESSAGE12_NO>::MessageContainerType;
   using PairwiseTripletMessage13Container = typename meta::at_c<typename FMC::MessageList, PAIRWISE_TRIPLET_MESSAGE13_NO>::MessageContainerType;
   using PairwiseTripletMessage23Container = typename meta::at_c<typename FMC::MessageList, PAIRWISE_TRIPLET_MESSAGE23_NO>::MessageContainerType;

public:
   template<typename SOLVER>
   tightening_mrf_constructor(SOLVER& pd)
      : MRF_PROBLEM_CONSTRUCTOR(pd)
   {}

   TripletFactorContainer* add_triplet_factor(const INDEX var1, const INDEX var2, const INDEX var3, const std::vector<REAL>& cost)
   {
      assert(var1<var2 && var2<var3);
      assert(var3<this->get_number_of_variables());
      assert(tripletMap_.find(std::array<INDEX,3>({var1,var2,var3})) == tripletMap_.end());
      
      assert(this->pairwiseMap_.find(std::make_tuple(var1,var2)) != this->pairwiseMap_.end());
      assert(this->pairwiseMap_.find(std::make_tuple(var1,var3)) != this->pairwiseMap_.end());
      assert(this->pairwiseMap_.find(std::make_tuple(var2,var3)) != this->pairwiseMap_.end());

      const INDEX factor12Id = this->pairwiseMap_.find(std::make_tuple(var1,var2))->second;
      const INDEX factor13Id = this->pairwiseMap_.find(std::make_tuple(var1,var3))->second;
      const INDEX factor23Id = this->pairwiseMap_.find(std::make_tuple(var2,var3))->second;

      TripletFactorContainer* t = this->lp_->template add_factor<TripletFactorContainer>(this->get_number_of_labels(var1), this->get_number_of_labels(var2), this->get_number_of_labels(var3));
      tripletFactor_.push_back(t);
      tripletIndices_.push_back(std::array<INDEX,3>({var1,var2,var3}));
      const INDEX factorId = tripletFactor_.size()-1;
      tripletMap_.insert(std::make_pair(std::array<INDEX,3>({var1,var2,var3}), factorId));

      LinkPairwiseTripletFactor<PairwiseTripletMessage12Container>(factor12Id,factorId);
      LinkPairwiseTripletFactor<PairwiseTripletMessage13Container>(factor13Id,factorId);
      LinkPairwiseTripletFactor<PairwiseTripletMessage23Container>(factor23Id,factorId); 

      //this->lp_->ForwardPassFactorRelation(this->get_pairwise_factor(factor12Id),t);
      //this->lp_->ForwardPassFactorRelation(this->get_pairwise_factor(factor13Id),t);
      //this->lp_->ForwardPassFactorRelation(t,this->get_pairwise_factor(factor23Id));

      //this->lp_->BackwardPassFactorRelation(this->get_pairwise_factor(factor23Id),t);
      //this->lp_->BackwardPassFactorRelation(this->get_pairwise_factor(factor13Id),t);
      //this->lp_->BackwardPassFactorRelation(t,this->get_pairwise_factor(factor12Id));
      return t;
   }
   template<typename PAIRWISE_TRIPLET_MESSAGE_CONTAINER>
   void LinkPairwiseTripletFactor(const INDEX pairwiseFactorId, const INDEX tripletFactorId)
   {
      using PairwiseTripletMessageType = typename PAIRWISE_TRIPLET_MESSAGE_CONTAINER::MessageType;

      typename MRFPC::PairwiseFactorContainer* const p = this->pairwiseFactor_[pairwiseFactorId];
      const INDEX pairwiseVar1 = this->pairwiseIndices_[pairwiseFactorId][0];
      const INDEX pairwiseVar2 = this->pairwiseIndices_[pairwiseFactorId][1];
      assert(pairwiseVar1 < pairwiseVar2);
      const INDEX pairwiseDim1 = this->get_number_of_labels(pairwiseVar1);
      const INDEX pairwiseDim2 = this->get_number_of_labels(pairwiseVar2);

      TripletFactorContainer* const t = tripletFactor_[tripletFactorId];
      const INDEX tripletVar1 = tripletIndices_[tripletFactorId][0];
      const INDEX tripletVar2 = tripletIndices_[tripletFactorId][1];
      const INDEX tripletVar3 = tripletIndices_[tripletFactorId][2];
      assert(tripletVar1 < tripletVar2 && tripletVar2 < tripletVar3);
      const INDEX tripletDim1 = this->get_number_of_labels(tripletVar1);
      const INDEX tripletDim2 = this->get_number_of_labels(tripletVar2);
      const INDEX tripletDim3 = this->get_number_of_labels(tripletVar3);
         
      assert(pairwiseDim1*pairwiseDim2 == p->GetFactor()->size());

      auto* m = this->lp_->template add_message<PAIRWISE_TRIPLET_MESSAGE_CONTAINER>(p, t, tripletDim1, tripletDim2, tripletDim3);
   }
   INDEX get_number_of_triplet_factors() const { return tripletFactor_.size(); }

   std::array<INDEX,3> get_triplet_indices(const INDEX factor_id)
   {
      assert(factor_id < get_number_of_triplet_factors());
      return tripletIndices_[factor_id];
   }

   void add_empty_pairwise_factor(const INDEX var1, const INDEX var2)
   {
      assert(this->pairwiseMap_.find(std::make_tuple(var1,var2)) == this->pairwiseMap_.end()); 
      this->add_pairwise_factor(var1, var2, matrix<REAL>(this->get_number_of_labels(var1), this->get_number_of_labels(var2), 0));
   }

   // do zrobienia: use references for pi
   bool add_tightening_triplet(const INDEX var1, const INDEX var2, const INDEX var3)//, const std::vector<INDEX> pi1, const std::vector<INDEX> pi2, const std::vector<INDEX> pi3)
   {
      assert(var1 < var2 && var2 < var3 && var3 < this->get_number_of_variables());
      if(tripletMap_.count(std::array<INDEX,3>({var1,var2,var3})) == 0) {
         // first check whether necessary pairwise factors are present. If not, add them.
         if(this->pairwiseMap_.find(std::make_tuple(var1,var2)) == this->pairwiseMap_.end()) {
            add_empty_pairwise_factor(var1,var2);
         }
         if(this->pairwiseMap_.find(std::make_tuple(var1,var3)) == this->pairwiseMap_.end()) {
            add_empty_pairwise_factor(var1,var3);
         }
         if(this->pairwiseMap_.find(std::make_tuple(var2,var3)) == this->pairwiseMap_.end()) {
            add_empty_pairwise_factor(var2,var3);
         }

         const auto tripletSize = this->get_number_of_labels(var1) *  this->get_number_of_labels(var2) * this->get_number_of_labels(var3);
         add_triplet_factor(var1,var2,var3, std::vector<REAL>(tripletSize,0.0));
         return true;
      } else {
         return false;
      }
   }

   INDEX add_triplets(const std::vector<triplet_candidate>& tc, const INDEX max_triplets_to_add = std::numeric_limits<INDEX>::max())
   {
      INDEX no_triplets_added = 0;
      for(auto t : tc) {
         if(add_tightening_triplet(t.i, t.j, t.k)) {
            ++no_triplets_added;
            if(no_triplets_added >= max_triplets_to_add) {
               break;
            }
         }
      }
      return no_triplets_added; 
   }

   INDEX Tighten(const INDEX noTripletsToAdd)
   {
      assert(noTripletsToAdd > 0);
      if(debug()) {
         std::cout << "Tighten mrf with cycle inequalities, no triplets to add = " << noTripletsToAdd << "\n";
      }

      //auto fp = [this](const INDEX v1, const INDEX v2, const INDEX v3) { return this->add_tightening_triplet(v1,v2,v3); }; // do zrobienia: do not give this via template, as Cycle already has gm_ object.

      triplet_search<typename std::remove_reference<decltype(*this)>::type> triplets(*this, eps);
      if(debug()) { std::cout << "search for triplets\n"; }
      auto triplet_candidates = triplets.search();
      if(debug()) { std::cout << "done\n"; }
      INDEX no_triplets_added = add_triplets(triplet_candidates, noTripletsToAdd);
      if(diagnostics()) { std::cout << "added " << no_triplets_added << " by triplet search\n"; }

      if(no_triplets_added < 0.2*noTripletsToAdd) {
         if(debug()) {
            std::cout << "----------------- do cycle search with k-projection graph---------------\n";
         }
         k_ary_cycle_inequalities_search<typename std::remove_reference<decltype(*this)>::type, false> cycle_search(*this, eps);
         triplet_candidates = cycle_search.search();
         if(debug()) { std::cout << "... done\n"; }
         const INDEX no_triplet_k_projection_graph = add_triplets(triplet_candidates, noTripletsToAdd-no_triplets_added);
         if(diagnostics()) {std::cout << "added " << no_triplet_k_projection_graph << " by cycle search in k-projection graph\n"; }
         no_triplets_added += no_triplet_k_projection_graph;

         // search in expanded projection graph
         if(no_triplets_added < 0.2*noTripletsToAdd) {
            if(debug()) {
               std::cout << "----------------- do cycle search with expanded projection graph---------------\n";
            }
            k_ary_cycle_inequalities_search<typename std::remove_reference<decltype(*this)>::type, true> cycle_search(*this, eps);
            triplet_candidates = cycle_search.search();
            if(debug()) { std::cout << "... done\n"; }
            const INDEX no_triplet_k_projection_graph = add_triplets(triplet_candidates, noTripletsToAdd-no_triplets_added);
            if(diagnostics()) { std::cout << "added " << no_triplet_k_projection_graph << " by cycle search in expanded projection graph\n";
            }
            no_triplets_added += no_triplet_k_projection_graph;
         }
      }

      if(debug()) {
         std::cout << "no triplets found, now search for triplets with guaranteed increase also smaller than " << eps << "\n";
      }
       
      assert(eps >= std::numeric_limits<REAL>::epsilon());

      if(no_triplets_added == 0) {
         triplet_search<typename std::remove_reference<decltype(*this)>::type> triplets(*this, std::numeric_limits<REAL>::epsilon());
         if(debug()) {
            std::cout << "search for triplets (any will do)\n";
         }
         auto triplet_candidates = triplets.search();
         if(debug()) {
            std::cout << "done\n";
         }
         no_triplets_added = add_triplets(triplet_candidates, noTripletsToAdd);
         if(diagnostics()) {
            std::cout << "added " << no_triplets_added << " by triplet search with no increase\n";
         }
      }

      if(no_triplets_added == 0) {
         if(debug()) {
            std::cout << "----------------- do cycle search with k-projection graph, add all---------------\n";
         }
         k_ary_cycle_inequalities_search<typename std::remove_reference<decltype(*this)>::type, false> cycle_search(*this, std::numeric_limits<REAL>::epsilon());
         triplet_candidates = cycle_search.search();
         if(debug()) { std::cout << "... done\n"; }
         const INDEX no_triplet_k_projection_graph = add_triplets(triplet_candidates, noTripletsToAdd-no_triplets_added);
         if(diagnostics()) {
            std::cout << "added " << no_triplet_k_projection_graph << " by cycle search in k-projection grapa with no increase\n";
         }
         no_triplets_added += no_triplet_k_projection_graph;
      }

      if(no_triplets_added == 0) {
         if(debug()) {
            std::cout << "----------------- do cycle search with expanded projection graph, add all---------------\n";
         }
         k_ary_cycle_inequalities_search<typename std::remove_reference<decltype(*this)>::type, true> cycle_search(*this, std::numeric_limits<REAL>::epsilon());
         triplet_candidates = cycle_search.search();
         if(debug()) { std::cout << "... done\n"; }
         const INDEX no_triplet_k_projection_graph = add_triplets(triplet_candidates, noTripletsToAdd-no_triplets_added);
         if(diagnostics()) {
            std::cout << "added " << no_triplet_k_projection_graph << " by cycle search in expanded projection graph with no increase\n";
         }
         no_triplets_added += no_triplet_k_projection_graph;
      }

      return no_triplets_added;
   }


protected:
   std::vector<TripletFactorContainer*> tripletFactor_;
   std::vector<std::array<INDEX,3>> tripletIndices_;
   std::map<std::array<INDEX,3>, INDEX> tripletMap_; // given two sorted indices, return factorId belonging to that index.
};

} // end namespace LP_MP

#endif // LP_MP_MRF_PROBLEM_CONSTRUCTION_HXX

