import xml.etree.ElementTree as ET

shape_size = [[1.0, 0.5]]

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
    # the parameter are the left up corner of the modle, shift it to the center
    n_pose.text = str(x + shape_size[shape_index][0]/2)+' '+str(y - shape_size[shape_index][1]/2)+' 0 0 -0 0'
    # todo how to change size of the model
    world.append(newshape)





def main():
    f = open("/home/hiwi03/Roomgenerator/generatelist.txt","r+")
    lines = f.readlines()
    # get the inital empty map with size 100*100
    tree = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/init.world')
    root = tree.getroot()
    world = root.find('world')
    for line in lines:
        newshape_info = line.split()
        #change it to the position in gazebo
        x = float(newshape_info[1])/10.0 -10.0
        y = 10.0 - float(newshape_info[0])/10.0
        insert_new_shape(x,y,int(newshape_info[2]),world)


    tree.write('/home/hiwi03/Roomgenerator/auto_gazebo/output.world')
    print('finished!')




main()