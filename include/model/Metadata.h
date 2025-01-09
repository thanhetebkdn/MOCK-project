#ifndef METADATA_H
#define METADATA_H

#include <map>
#include <string>

class Metadata {
private:
    std::map<std::string, std::string> metadata;

public:
    std::map<std::string, std::string> getMetaData() const;
    void setData(const std::map<std::string, std::string>& data);
    std::string getValue(const std::string& key) const;
    void setValue(const std::string& key, const std::string& value);
    void displayMetadata() const;
};

<<<<<<< HEAD
#endif // METADATA_H
=======
#endif
>>>>>>> 9e80c8d50e0ec264a4430852ad12d7a0c788daf7
