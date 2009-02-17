#include "link.h"


bool Link::Prescreening_Sort_(BasisShell* shellA, BasisShell* shellB) {

  index_t a_ind = shellA->start_index();
  index_t b_ind = shellB->start_index();

  // how to get index for mu?  
  return ((shell_max_[a_ind] * density_mat_.ref(current_mu_, a_ind)) < 
          (shell_max_[b_ind] * density_mat_.ref(current_mu_, b_ind)));

}


void Link::ComputeFockMatrix() {

  // Find "significant" bra and ket shell pairs
  // These need to be ordered in terms of their Schwartz estimates
  // I think shell_pair_list_ needs to be a list of lists
  // One list of shell pairs for each mu
  num_shell_pairs_ = eri::ComputeShellPairs(&shell_pair_list_, shell_list_, 
                                            shell_pair_cutoff_, &shell_max_);
  
  // Find significant \nu for all \mu
  
  // I am supposed to loop over all mu in significant bra shell pairs and over 
  // all nu in significant ket shell pairs
  // Maybe I should just loop over the shells themselves and assume that each
  // shell appears in at least one significant shell pair
  for (index_t i = 0; i < num_shells_; i++) {
  
    current_mu_ = i;
  
    index_t next_ind = 0;

    ArrayList<index_t> significant_nu_index;
    significant_nu_index.Init(num_shells_);
        
    for (index_t j = 0; j < num_shells_; j++) {
    
      // I think this threshold should be the same as the others 
      // Not sure if this is the right density matrix entry
      if (density_mat_.ref(i,j) * shell_max_[i] * shell_max_[j] > threshold_) {
        
        // Store significant j for each i
        // What is the best way to do this?  This list will have to be sorted
        // This fails if there are more than num_shells_/2 significant elements
        significant_nu_index[next_ind] = j;
        next_ind++;
      
      }
    
    } // for j
    
    
    significant_mu_pairs_[i] = (BasisShell**)malloc(next_ind); 
    BasisShell** significant_nu_for_mu = significant_mu_pairs_[i];
    
    
    for (index_t k = 0; k < next_ind; k++) {
    
      significant_nu_for_mu[k] = shell_list_.begin() + 
                                  significant_nu_index[k];
    
    } // for k
    
    // sort significant_nu_for_mu
    
    std::sort(significant_nu_for_mu, significant_nu_for_mu+next_ind, 
              &Link::Prescreening_Sort_);
    
  
  } // for i
  
  // loop over bra shell pairs
  
  /*
  for (index_t mulambda_ind = 0; mulambda_ind < num_shell_pairs_; 
       mulambda_ind++) {
  
    // loop over \nu for the current \mu
    for (index_t nu_ind = 0; nu_ind < num_nu; nu_ind++) {
  
      // loop over \sigma and screen to see if this integral is necessary
      // how to figure out which sigmas appear in shell pairs with 
  
    } // loop nu_ind
  
    // loop over nu corresponding to lambda and do same
    // why is this necessary as well?  
  
    // compute significant integrals



  } // loop mulambda_ind (over bra shell pairs)

   */

} // ComputeFockMatrix()



void Link::OutputExchangeMatrix(Matrix* exc_out) {

  exc_out->Copy(exchange_matrix_);
  
} // OutputFockMatrix