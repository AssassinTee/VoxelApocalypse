/**
@Author Marvin Winkens
@Email m.winkens@fz-juelich.de
*/

/**Basics of the octal tree behind the scenes of the map
Every Node will derive from this one. It is template based,
so i can put in here everything in.
This File is ment for copy, so use it, but don't remove this or my name*/
#include <vmath.h>//vectors

#include <boost/filesystem.hpp>//to create and destroy directories
#include <boost/archive/binary_oarchive.hpp>//for output archives
#include <boost/archive/binary_iarchive.hpp>//for input archives
#include <boost/serialization/base_object.hpp>//for derived classes
#include <boost/serialization/split_member.hpp>//for save and load function, not needed everywhere

#define GAMEDIM 3
#define OCTAL 8
enum CNodeType {//This represents the hierachical order of the octree
    NODE_BRANCH=0,//This ones just contains position information
    NODE_BASE,//This one contains mapdata if existent
    NODE_SUB,//This one stores other data, if needed
};

//#####STRUCTURE##################
//               Br                 Br = Branch Node
//             /    \               Ba = Base Node
//           /        \             S  = Sub Node
//         /            \
//       Br              Br
//     /   \           /   \
//   Ba     Ba       Ba     Ba
// / /\ \ / /\ \   / /\ \ / /\ \
//S S S S S S S S S S S S S S S S
//
//Actually this tree is much bigger, because every Node has EIGHT childen
//this is just an easy representation
//
//################################

template <class CElement>
class CMapNode
{
    public:
        virtual ~CMapNodeBase() {};

        //Getter & Setter
        virtual CMapNode<CElement>* getChild(unsigned int index){return ma_Child[index];}//This may returns null
        virtual CMapNode<CElement>* getParent() {return mp_Parent;}
        virtual CNodeType getType() = 0;

        vector3_base<double> getMin() {return mv3_minPos;}
        vector3_base<double> getMax() {return mv3_maxPos;}

        //rendering stuff
        virtual void snap(unsigned int lod) = 0;// lod = level of detail, where 0 is the highest level.


        //Operators
        //template <class AnotherCElement>//Eleminates shadowing?!
        //friend std::ostream& operator<<(std::ostream& os, CNode<AnotherCElement>& node);//needed for serialization

        //Serialization
        //virtual void serialize() = 0;
        //virtual void serializeLeafs() = 0;
        //virtual void deserialize() = 0;

        //Boost serialization of base class
        friend class boost::serialization::access;//This will be needed later

        //just serializes the information of this exact node.
        template <class Archive>
        void serialize( Archive & ar, const unsigned int version)
        {
            ar & mv3_minPos.x;
            ar & mv3_minPos.y;
            ar & mv3_minPos.z;
            ar & mv3_maxPos.x;
            ar & mv3_maxPos.y;
            ar & mv3_maxPos.z;
        }

        bool isInNode(vector3_base<double> pos, unsigned int radius=0);
    private:
    protected:
        //unsigned int m_Level;//which level has this tree element//I add this when i really need this
        vector3_base<double> mv3_minPos;//The lower corner of the octant
        vector3_base<double> mv3_maxPos;//The upper corner of the octant
        CMapNode* ma_Child[OCTAL] {0};//All children, maybe nullptr
        CMapNode* mp_Parent;//The parent, root has nullptr
};
