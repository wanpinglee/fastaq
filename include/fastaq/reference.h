#ifndef REFERENCE_H_
#define REFERENCE_H_

#include <map>
#include <vector>
#include <iostream>

#include "read.h"

namespace Fastaq {
struct SReference
{
  SRead read;
  unsigned int length;
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
  inline std::string GetSubString(const std::string &chr_name, unsigned int pos, unsigned int length) const;
  // Get the length of the reference by id.
  inline unsigned int GetReferenceLength(int id) const;
  // Get the length of the reference by name.
  inline unsigned int GetReferenceLength(const char * name) const;
  // Get the id of the reference by name.
  inline int GetReferenceId(const char * name) const;
  // Clean
  inline void Clean();

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

inline unsigned int CReference::GetReferenceLength(int id) const
{
  std::string name = ref_names_[id];
  std::map<std::string, SReference>::const_iterator ite = refs_.find(name);
  if (ite == refs_.end() || ite->second.id != id)
	  return -1;

  return ite->second.length;
}

inline unsigned int CReference::GetReferenceLength(const char *name) const
{
  std::map<std::string, SReference>::const_iterator ite = refs_.find(name);
  if (ite == refs_.end())
	  return -1;
  return ite->second.length;
}

inline std::string CReference::GetSubString(const std::string &chr_name, unsigned int pos, unsigned int length) const
{
  std::map<std::string, SReference>::const_iterator ite = refs_.find(chr_name);
  if (ite == refs_.end() || pos >= ite->second.length)
	  return std::string();
  
  return ite->second.read.bases.substr(pos, length);
}

inline int CReference::GetReferenceId(const char * name) const
{
  std::map<std::string, SReference>::const_iterator ite = refs_.find(name);
  if (ite == refs_.end())
	return -1;
  return ite->second.id;
}

inline void CReference::Clean() {
  last_id_ = 0;
  refs_.clear();
  ref_names_.clear();
}
}
#endif //REFERENCE_H_
