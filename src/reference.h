#ifndef REFERENCE_H_
#define REFERENCE_H_

#include <map>
#include <vector>
#include <iostream>

#include "../include/read.h"

struct SReference
{
  SRead read;
  int length;
  int id;
};

class CReference
{
 public:
  CReference();
  ~CReference();

  // Append a new reference.
  bool AddReference(const char *pName, const char *pSeq);
  // Get reference names in order.
  inline void GetReferenceNames(std::vector<std::string>* pRef_name) const;
  // Get reference count.
  inline unsigned int GetReferenceCount() const {return refs_.size();};
  // Get a sub string
  inline std::string GetSubString(const std::string &chr_name, int pos, int length) const;
  // Get the length of the reference by id.
  inline int GetReferenceLength(int id) const;
  // Get the length of the reference by name.
  inline int GetReferenceLength(const char * name) const;

 protected:
  std::map<std::string, SReference> refs_; //name and SReference
  std::vector<std::string> ref_names_; // names in order

 private:
  int last_id_;
};

inline void CReference::GetReferenceNames(std::vector<std::string>* pRef_name) const
{
  *pRef_name = ref_names_;
}

inline int CReference::GetReferenceLength(int id) const
{
  std::string name = ref_names_[id];
  std::map<std::string, SReference>::const_iterator ite = refs_.find(name);
  if (ite == refs_.end() || ite->second.id != id)
	  return -1;

  return ite->second.length;
}

inline int CReference::GetReferenceLength(const char *name) const
{
  std::map<std::string, SReference>::const_iterator ite = refs_.find(name);
  if (ite == refs_.end())
	  return -1;
  return ite->second.length;
}

inline std::string CReference::GetSubString(const std::string &chr_name, int pos, int length) const
{
  if (length <= 0)
	  return std::string();
	  
  std::map<std::string, SReference>::const_iterator ite = refs_.find(chr_name);
  if (ite == refs_.end() || pos >= ite->second.length)
	  return std::string();
  
  return ite->second.read.bases.substr(pos, length);
}

#endif //REFERENCE_H_
