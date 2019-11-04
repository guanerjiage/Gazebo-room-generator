import xml.etree.ElementTree as ET

def insert_new_shape(x, y, shape_index, world):
    # get the handler of tag model in newshape file
    if shape_index == 0:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape0.sdf')
    else:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape0.sdf')
    newshape_root = newshapefile.getroot()
    newshape = newshape_root.find('model')

    # changing location and adding shape into the world file
    n_pose = newshape.find('pose')
    # in the form of (x,y,z,alpha,beta,garmma)
    n_pose.text = str(x)+' '+str(y)+' 0 0 -0 0'
    # todo how to change size of the model
    world.append(newshape)





def main():
    # get the inital empty map with size 100*100
    tree = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/init.world')
    root = tree.getroot()
    world = root.find('world')
    # the location here is the center of shape, need to be transformed
    insert_new_shape(1,2,0,world)

















main()