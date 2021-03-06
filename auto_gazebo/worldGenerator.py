import xml.etree.ElementTree as ET

def insert_new_shape(x, y, shape_index, world):
    
    # get the handler of tag model in newshape file
    if shape_index == 0:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape0.sdf')
    elif shape_index == 1:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape1.sdf')
    elif shape_index == 2:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape2.sdf')
    elif shape_index == 3:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape3.sdf')
    elif shape_index == 4:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape4.sdf')
    elif shape_index == 5:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape5.sdf')
    elif shape_index == 6:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape6.sdf')
    elif shape_index == 7:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape7.sdf')
    elif shape_index == 8:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape8.sdf')
    elif shape_index == 9:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/cylinder_d=30.sdf')
    elif shape_index == 10:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/cylinder_d=50.sdf')
    elif shape_index == 11:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/ball_d=50.sdf')
    elif shape_index == 12:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/box_d=50.sdf')
    elif shape_index == 13:
        newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/box_d=20.sdf')
    
    newshape_root = newshapefile.getroot()
    newshape = newshape_root.find('model')
    # changing location and adding shape into the world file
    if shape_index>=9:
        n_link = newshape.find('link')
        n_pose = n_link.find('pose')
    else:
        n_pose = newshape.find('pose')
    # in the form of (x,y,z,alpha,beta,garmma)
    n_pose.text = str(x) +' '+str(y)+' 0 0 -0 0'
    # print(n_pose.text)
    # todo how to change size of the model
    world.append(newshape)





def main():
    f = open("/home/hiwi03/Roomgenerator/generatelist.txt","r+")
    lines = f.readlines()
    tree = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/init80.world')
    root = tree.getroot()
    world = root.find('world')
    for line in lines:
        newshape_info = line.split()
        insert_new_shape(newshape_info[0],newshape_info[1],int(newshape_info[2]), world)


    tree.write('/home/hiwi03/Roomgenerator/auto_gazebo/80*80_occu=30_1.world')
    print('finished!')




main()