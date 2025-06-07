#pragma once
#include "pugixml.hpp"
#include <vector>
#include <string>
#include <fstream>

static std::vector<std::string_view> output_col_names={ "Index", "Initial", "Final", "Min", "Max", "Mean", "Accumulated", "StoreFlag" };
struct Output_InXml { std::string name{ "" }; int Index{ -1 }; double dvalues[8]{0.,0.,0.,0.,0.,0.,0.,0.}; int flag{ -1 }; };
struct Multi_InXml {
	int numrepeat; std::string group; std::string name; int tabindex; int imethod; int imon; double fmin;
	double fmax; double fstart; int nCount; std::string group_dep; std::string name_dep; int tabindex_dep; std::vector<double> tablevalues;
	std::string db_opt; std::string db_key;
};
class VALv;
class CHPARv;
class Sw;
class SimB;
class ModelBase;
class CommonModelInfo;
class NE;
class Sim_Xml {
public:
    Sim_Xml() {
        m_IsOpen = true;
        m_xmlfileName = "CoupModel_Sim.xml";
        OpenRegister();
        if (!m_register.is_open()) OpenRegister();
    };

    ~Sim_Xml()  noexcept { OpenRegister(false); };

    bool OpenRegister(bool read = true) {

        std::string line_str;
        //if (m_register.is_open()) {
        if (read) {
            if (ReadRegisterFile() == std::string::npos) {
                return false;
            }
        } else {
            m_register.clear();
            WriteRegisterFile();
            m_register.close();
        }
        return true;
    };

    bool WriteRegisterFile()  noexcept {

        return false;
    }

    void setColumnAtt(std::vector<std::string> att, std::string Tag = "ColumnAtt") {
        /*	auto child = demoData->getChildByName(Tag);
            if (child == nullptr) {
                child = demoData->createNewChildElement(Tag);
                for(size_t i=0; i<att.size(); i++)
                    child->setAttribute(string(att[i]), int(i+1));

            }*/
    }

    int GetInt(const char *module, const char *type, std::string name, int default_value)   noexcept  {
/*		auto child3 = getLevel3(module, type, name);
		if (child3 == nullptr) {
			child3 = getLevel2(module, type)->createNewChildElement(name);
			child3->setAttribute("Value", default_value);
			return default_value;
		}
		else
			return child3->getIntAttribute("Value");*/
        return 1;
    };

    void setValue(const char *module, const char *type, const char *name, int newvalue) {
        auto child3 = getLevel3(module, type, name);
        if (child3 == nullptr) {
            //child3 = getLevel2(module, type)->createNewChildElement(name);
            //child3->setAttribute("Value", newvalue);
        }

        //child3->setAttribute("Value" , newvalue);
    };

    pugi::xml_node getLevel3(const char *module, const char *type, const char *name) {
        pugi::xml_node node;

        node = doc.child(module).child(type).child(name);
        return node;
    }

    pugi::xml_node getLevel2(const char *module, const char *type) {
        pugi::xml_node node;
        node = doc.child(module).child(type);
        return node;
    }

    std::string getstring(const char *name, std::string default_value) {
        auto child = currentList.child(name);
        /*if (child == nullptr) {
            child = currentList->createNewChildElement(name);
            child->setAttribute("Value", default_value);
            return default_value;
        }
        else
            return child->getstringAttribute("Value").toStdstring();*/
    };

    std::vector<std::pair<std::string, std::string>> getAllCurrentListstring() noexcept {
        std::vector<std::pair<std::string, std::string>> out;
        // Implementation of the method goes here
        return out;
    };


    void setValue(const char *name, std::string newvalue)  noexcept {
        /*	auto child = currentList->getChildByName(name);
            if (child == nullptr) {
                child = currentList->createNewChildElement(name);
                child->setAttribute("Value", newvalue);
            }
            else
                child->setAttribute("Value", newvalue);*/
    };

    void setTextValue(const char *name, std::string newvalue)  noexcept {
        /*	auto child = currentList->getChildByName(name);
            if (child == nullptr) {
                child = currentList->createNewChildElement("Text");
                child->createTextElement(newvalue);
            }
            else
                child->addTextElement( newvalue);*
        };*/
    }
        double getDouble(const char *name, double default_value) {
            auto child = currentList.child(name);
            /*if (child == nullptr) {
                child = currentList->createNewChildElement(name);
                child->setAttribute("Value", default_value);
                return default_value;
            }
            else
                return child->getDoubleAttribute("Value");*/
            auto attr = child.attribute(name);
            if (attr == nullptr) return default_value;
            attr.as_double(default_value);
        };
        void setValue(const char *name, double newvalue) {
            auto child = currentList.child(name);

        };
        bool getbool(const char *name, bool default_value) {
            auto child = currentList.child(name);

/*            if (child == nullptr) {
                child = currentList->createNewChildElement(name);
                child->setAttribute("Value", default_value);
                return default_value;
            } else
                return child->getDoubleAttribute("Value");*/
            return false;
        };
        void setValue(const char *name, bool newvalue) {
            auto child = currentList.child(name);
       /*     if (child == nullptr) {
                child = currentList->createNewChildElement(name);
                child->setAttribute("Value", newvalue);
            } else
                child->setAttribute("Value", newvalue);*/
        };
        int getInt(const char *name, int default_value) {
            auto child = currentList.child(name);
            return 1;
        };

        void setValue(const char *name, int newvalue) {
            auto child = currentList.child(name);
        };
        void setTable(const char *module, const char *type, const char *name, std::vector<std::string> colnames,
                      std::vector<std::vector<double>> values)  noexcept {

           /* auto child3 = getLevel3(module, type, name);
            if (child3 == nullptr) {
                child3 = getLevel2(module, type)->createNewChildElement(name);
            }
            for (size_t i = 0; i < colnames.size(); i++) {
                child3->setAttribute(string(colnames[i]), int(i + 1));
            }
            child3->deleteAllChildElements();
            for (size_t i = 0; i < values.size(); i++) {
                std::string tag = "Row_" + std::string(int(i + 1));
                auto child4 = new XmlElement(tag);
                for (size_t j = 0; j < values[i].size(); j++) {
                    child4->setAttribute(string(colnames[j]), values[i][j]);
                }
                child3->addChildElement(child4);
            }*/
        }

        void setOutPutRow(const char *module, const char *type, const char *name, int index_output, int storeflag,
                          std::vector<double> values)  noexcept {

          /*  auto child3 = getLevel3(module, type, name);
            if (child3 == nullptr) {
                child3 = getLevel2(module, type)->createNewChildElement(name);
            }

            for (size_t i = 0; i < output_col_names.size(); i++) {
                child3->setAttribute(string(output_col_names[i]), int(i + 1));
            }
            std::string tag = "Row";
            auto child4 = new XmlElement(tag);
            if (index_output >= 0) child4->setAttribute(string(output_col_names[0]), index_output);
            for (size_t i = 0; i < values.size(); i++) {
                child4->setAttribute(string(output_col_names[i + 1]), values[i]);
            }
            child4->setAttribute(string(output_col_names[7]), storeflag);
            child3->addChildElement(child4);*/
        }
        std::vector<Output_InXml> getOutputs(const char *type)  noexcept {
            std::vector<Output_InXml> out;
/*            Output_InXml node;
            auto child = demoData->getChildByName(type);
            if (child == nullptr) return out;
            auto child2 = child->getFirstChildElement();
            while (child2 != nullptr) {
                auto group = child2->getTagName().toStdstring();
                auto child3 = child2->getFirstChildElement();

                while (child3 != nullptr) {

                    auto child4 = child3->getFirstChildElement();
                    auto rows = child3->getNumChildElements();
                    node.name = child3->getTagName().toStdstring();
                    for (int row = 0; row < rows; row++) {
                        child4 = child3->getChildElement(row);
                        node.Index = child4->getIntAttribute("Index");
                        for (size_t i = 0; i < 6; i++) {
                            node.dvalues[i] = child4->getDoubleAttribute(output_col_names[i + 1]);
                        }
                        node.flag = child4->getIntAttribute("StoreFlag");
                        out.push_back(node);
                    }
                    child3 = child3->getNextElement();
                }
                child2 = child2->getNextElement();
            }*/
            return out;
        };
        void setValidationRow(VALv val);
        std::vector<VALv> getValidationRows();
        void setWithinRunRow(CHPARv ch);
        std::vector<CHPARv> getWithinRunRows(ModelBase *);
        void setMultiRunRow(int NumberOfRepeatitionsWithinDimension, int nCount, int numpar,
                            std::string group_psw, const char *name_psw, int TabIndex, int imethod, int imon,
                            std::vector<double> table, std::string selection_pcp, std::string keyselection_pcp,
                            std::string group_psw_dep, const char *name_psw_dep, int ind_dep,
                            double fmin, double fmax, double fstart, int i1);
        std::vector<Multi_InXml> getMultiRunRows();


        bool IsRegisterOpen()  noexcept { return m_IsOpen; };
        void setModelGroup(std::vector<SimB*>, CommonModelInfo *);
        void setModelItems(std::vector<SimB*>, CommonModelInfo *);
        void setModelNE_All(std::vector<NE*>);

        void setRegister(const std::string tag, const int value)  noexcept {
            /*auto child = dataList.node(tag);
            if (child != nullptr)
                child->setAttribute("Value", value);*/
        };
        void setRegister(const std::string tag, const std::string value)  noexcept {
            //dataList->getChildByName(tag)->setAttribute("Value", value);
        };
        void setRegister(const std::string tag, const double value)  noexcept {
            //	dataList->getChildByName(tag)->setAttribute("Value", value);
        };
        int getIntRegister(const std::string tag)  noexcept {
            //	return dataList->getChildByName(tag)->getIntAttribute("Value");
        };
        std::string getstringRegister(const std::string tag)  noexcept {
            //	return dataList->getChildByName(tag)->getstringAttribute("Value");
        };
        double getDoubleRegister(const std::string tag)  noexcept {
            //	return dataList->getChildByName(tag)->getDoubleAttribute("Value");
        };
        void setHiddenData(const std::string tag, const int value)  noexcept {
            /*	auto child = dataStore->getChildByName(tag);
                if (child != nullptr)
                    child->setAttribute("Value", value);
                else {
                    child = dataStore->createNewChildElement(tag);
                    child->setAttribute("Value", value);
                }*/
        };
        void setHiddenData(const std::string tag, const std::string value)  noexcept {
            /*auto child = dataStore->getChildByName(tag);
            if (child != nullptr)
                 child->setAttribute("Value", value);
            else {
              child = dataStore->createNewChildElement(tag);
              child->setAttribute("Value", value);
            }*/
        };
        void setHiddenData(const std::string tag, const double value) noexcept {
            /*auto child = dataStore->getChildByName(tag);
            if (child != nullptr)
                child->setAttribute("Value", value);
            else {
                child = dataStore->createNewChildElement(tag);
                child->setAttribute("Value", value);
            }*/
        };
        void setHiddenChildData(const std::string tag, const double value)  noexcept {
            /*auto child = dataStore->getChildByName(tag);
            if (child != nullptr)
                child->setAttribute("Value", value);
            else {
                child = dataStore->createNewChildElement(tag);
                child->setAttribute("Value", value);
                auto child2= child->createNewChildElement("Options");
                        child2->setAttribute("Value", value);
            }*/
        };
        int getHiddenIntData(const std::string tag)  noexcept {
            /*auto child = dataStore->getChildByName(tag);
            if (child != nullptr)
                return child->getIntAttribute("Value");
            else
                return 0;*/
        };

        std::string getHiddenstringData(const std::string tag)  noexcept {
            /*auto child = dataStore->getChildByName(tag);
            if (child != nullptr)
                return child->getstringAttribute("Value");
            else
                return "";*/
        };
        double getHiddenDoubleData(const std::string tag) noexcept {
            /*auto child = dataStore->getChildByName(tag);
            if (child != nullptr)
                return child->getDoubleAttribute("Value");
            else
                return double(MISSING);*/
        };

        bool setFileName(const char *name)  noexcept {
//            if (name.find(":") == std::string::npos)
  //              return false;
          
            doc.load_file(name);
            m_xmlfileName = name;

            return true;

        };


        void saveFile() {
            WriteRegisterFile();
        };
        size_t ReadRegisterFile() {

            return 0;
        }
        std::vector<std::pair<std::string, int>> getAllInt(const char *type);
        std::vector<std::pair<std::string, double>> getAllDouble(const char *type);
        std::vector<std::pair<std::string, std::string>> getAllstring(const char *type);
        std::vector<std::string> getAllNames(const char *type);
        std::vector<std::pair<std::string, std::string>> getAllChangedItems();
        void insert_ChangedNames(std::vector<std::pair<std::string, std::string>>);
        std::vector<std::pair<std::string, std::pair<std::vector<std::string>, std::vector<std::vector<double>>>>> getAllDoubleTables(const char *type);
        void setSimFileName(std::string file) { m_xmlSimfileName = file; };
        void setModelFileName(std::string file) { m_xmlModelfileName = file; };
        std::string getSimFileName() { return m_xmlSimfileName; };
        std::string getModelFileName() { return m_xmlModelfileName; };



        private:
        pugi::xml_document doc;
        // A pointer to the sub-node of demoData that contains the list of columns
        pugi::xml_node dataList;
        pugi::xml_node currentList;

        pugi::xml_node dataStore;
        pugi::xml_node comboBoxItems;


        bool m_IsOpen;
        bool m_typeAsHighestLevel{true};
        std::fstream m_register;
        std::string m_xmlfileName;
        std::string m_xmlSimfileName;
        std::string m_xmlModelfileName;
};