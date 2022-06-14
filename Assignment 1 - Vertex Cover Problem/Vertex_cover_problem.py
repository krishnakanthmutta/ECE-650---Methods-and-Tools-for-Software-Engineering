#!/usr/bin/env python
# http://docs.python.org/tutorial/
# http://docs.python.org/library/re.html

# a "test01" (0,0) (10,10)
# a "test02" (10,0) (0,10)


#************************************************************
# imports
#************************************************************

import re
import sys
import math

#Street name
#name_rx = '\".+?\"'
#name_rx = '\"[a-zA-Z]\s*[a-zA-Z]\s*[a-zA-Z]\"$'
###name_rx = '\"[a-zA-Z]+[a-zA-Z\s]*[a-zA-Z]\"'
name_rx = '\"[A-Za-z\s]*[A-Za-z]\"'
name_chk = re.compile(name_rx)
name_rx_space = "  "
name_space_chk = re.compile(name_rx_space)
#Coordinates
num_rx = '-?\d+'
#coord_rx = r'\(\s*'+num_rx+'\s*,\s*'+num_rx+'\s*\)'
coord_rx = r'\('+num_rx+','+num_rx+'\)'
coord_chk = re.compile(coord_rx)

#add
#cmd_a_rx = '\s*add\s+'+name_rx+'\s+('+coord_rx+'\s*){2,}\s*$'
cmd_a_rx = 'add\s'+name_rx+'\s'+coord_rx+'\s(' + coord_rx + '\s){0,}' + coord_rx + '$'
#cmd_a_rx = 'add\s\"'+name_rx+'\"\s'+coord_rx+'\s(' + coord_rx + '\s){0,}' + coord_rx + '$'
#cmd_a_rx = '\s*add\s+'+name_rx+'\s+('+coord_rx+')\s+' + ('('+ coord_rx+')\s+ {0,}')  + '('+ coord_rx + ')$'
cmd_a_chk = re.compile(cmd_a_rx)

#mod
cmd_m_rx = 'mod\s'+name_rx+'\s'+coord_rx+'\s(' + coord_rx + '\s){0,}' + coord_rx + '$'
cmd_m_chk = re.compile(cmd_m_rx)

#rm
cmd_r_rx = 'rm\s'+name_rx+'$'
cmd_r_chk = re.compile(cmd_r_rx)

#gg
cmd_g_rx = 'gg$'
cmd_g_chk = re.compile(cmd_g_rx)

inp_lines = []

lines_dict ={}
list1 = []
list2 = []
final_list = []
vertices_final = []
int_coord_final = []
street_name = []
street_coords = []
coord_with_int = []
edges1 = []
edges_f = []

#************************************************************
# defines
#************************************************************

def line(p1, p2):
    x = p1[1] - p2[1]
    y = p2[0] - p1[0]
    z = (p1[0]*p2[1]) - (p2[0]*p1[1])
    return x, y, -z

def intersect(l1, l2):
    d  = l1[0] * l2[1] - l1[1] * l2[0]
    dx = l1[2] * l2[1] - l1[1] * l2[2]
    dy = l1[0] * l2[2] - l1[2] * l2[0]
    if d != 0:
        x = dx / d
        y = dy / d
        return round(x,2),round(y,2)
        #return x,y
    else:
        return False

class Street():
    def __init__(self, name, coords):
        self.name = name
        self.coords = coords
        # print(self.name)
        # print(self.coords)
    
    def street_storing(name, coords):
        street_name.append(name.lower())
        street_coords.append(coords)
        #print(street_name)
        #print(street_coords) 

    def street_search(self):
        for key in street_name:
            if key == self.name.lower():
                return True
        return False

    def name_check(self):
        if self.name.isspace():
            return 1

        for i in str(self.name):
            if i.isalpha() != 1 and i.isspace() != 1:
                return 1
            else:
                p=0
        if p==0:
            return 0
    
    def coords_check(self):
        c=0
        for j in self.coords:
            c+=1
        return c       

#to generate the index of the street to reference the coordinates list
def street_index(name):
    for i in range(len(street_name)):
        if street_name[i] == name:
            return i

#modify street
def modify_street(name, coords):
    i = street_index(name.lower())
    street_coords[i] = coords
    #print(street_name)
    #print(street_coords) 

#delete street
def delete_street(name):
    i = street_index(name.lower())
    del street_name[i]
    del street_coords[i]
    # print(street_name) 
    # print(street_coords)

#to check if the coordinate is already in the final vertex list 
def check_vertex(coords):
    j=0
    for i in range(len(vertices_final)):
        if vertices_final[i] == coords:
            j=1
    if j !=1:
        return 1
    else:
        return 0

#to calculate the distance between two points
def dista(a,b):
    return math.sqrt((a[0] - b[0])**2 + (a[1] - b[1])**2)

#to check if the intersection point is between the two coordinates and 0.05 for rounding off error
def between(a,c,b):
    if round(dista(a,c),2) + round(dista(c,b),2) <= round(dista(a,b),2) + 0.05:
        return 1
    else:
        return 0

#to compute the edges from the list of vertcies and intersection points
def edges_compute(coord_with_int):
    edges_f = []
    for x in range(len(street_name)):
        for y in range(len(coord_with_int[x])-1):
            edges1= list()
            if ((coord_with_int[x][y] in vertices_final) and (coord_with_int[x][y+1] in vertices_final)) and ((coord_with_int[x][y] in int_coord_final) or (coord_with_int[x][y+1] in int_coord_final)):
                edges1.append(coord_with_int[x][y])
                edges1.append(coord_with_int[x][y+1])
                edges_f.append(edges1)
    return(edges_f)

#to print the vertices and edges onto the output terminal
def print_vertices_edges(vertices_final, edges_f):
    v_final = {}
    #making a dictiory of vertices to print in the required format
    print("V = {")
    for p in range(len(vertices_final)):
        key = p+1
        v_final[key] = vertices_final[p]
        #print("  " + str(key) + ":  " + str(v_final[key]) )
        print("  " + str(key) + ":  (" + str(v_final[key][0]) + "," + str(v_final[key][1]) + ')')
        #print("  " + str(key) + ":  (" + str(round(v_final[key][0],2)) + str(round(v_final[key][1],2)) + ')' )
    print("}")
    

    #printing edges
    print("E = {")
    for q in range(len(edges_f)):
        for s in range(len(v_final)):
            r = s+1
            if edges_f[q][0] == v_final[r]:
                # print("<"+ str(r) + ",")
                f1 = r
        for t in range(len(v_final)):
            u = t+1    
            if edges_f[q][1] == v_final[u]:
                #print(str(r) + "> \n")
                f2 = u
        if q == len(edges_f)-1:
            print("  <"+ str(f1) + "," + str(f2) + ">")
        else:
            print("  <"+ str(f1) + "," + str(f2) + ">,")
    print("}")

#to compute the vertices from the list of coordinates
def vertices_compute():
    coord_with_int = [x[:] for x in street_coords]
    vertices_final.clear()
    int_coord_final.clear()
    for a in range(len(street_name)):
        for b in range(a+1,len(street_name)):
            for c in range(len(street_coords[a])-1):
                #print("in c")
                v1 = street_coords[a][c]
                v2 = street_coords[a][c+1]
                for d in range(len(street_coords[b])-1):
                    #print("in d")
                    v3 = street_coords[b][d]
                    v4 = street_coords[b][d+1]
                    #print(v1,v2,v3,v4)
                    l1 = line(v1,v2)
                    l2 = line(v3,v4)
                    coord_int = intersect(l1,l2)
                    #print(coord_int)
                    if coord_int !=0 and between(v1,coord_int,v2) == 1 and between(v3,coord_int,v4) == 1:
                        if check_vertex(v1):
                            vertices_final.append(v1)
                        if check_vertex(v2):
                            vertices_final.append(v2)
                        if check_vertex(v3):
                            vertices_final.append(v3)
                        if check_vertex(v4):
                            vertices_final.append(v4)
                        if check_vertex(coord_int):
                            vertices_final.append(coord_int)
                        int_coord_final.append(coord_int)
                        
                        zt1 = 0
                        zt2 = 0

                        #preparing a new list with coordinates and intersection points in order
                        for co1 in range(len(coord_with_int[a])):
                            if coord_with_int[a][co1] == v1:
                                pt1 = co1 + 1
                        for co3 in range(len(coord_with_int[a])):
                            if coord_with_int[a][co3] == coord_int:  
                                zt1 = 1
                        
                        #verifying the position of the intersection point with the existing intersection points
                        if coord_with_int[a][pt1] in int_coord_final and dista(v1,coord_int) > dista(v1,coord_with_int[a][pt1]):                      
                            pt1 += 1 
                        
                        if zt1 !=1:
                            coord_with_int[a].insert(pt1 ,coord_int)
                            

                        for co2 in range(len(coord_with_int[b])):
                            if coord_with_int[b][co2] == v3:
                                pt2 = co2 + 1
                        for co4 in range(len(coord_with_int[b])):
                            if coord_with_int[b][co4] == coord_int:    
                                zt2 = 1
                        
                        if coord_with_int[b][pt2] in int_coord_final and dista(v3,coord_int) > dista(v3,coord_with_int[b][pt2]):
                            pt2 += 1 
                                
                        if zt2 != 1:
                            coord_with_int[b].insert(pt2 ,coord_int)
                        
                        zt1 = 0
                        zt2 = 0
                        


    # print("Vertices final")
    # print(vertices_final)
    # print("Int coord final")
    # print(int_coord_final)
    # print("Coord with int")
    # print(coord_with_int)

    #computing the edges and printing vertices and edges
    edges_f = edges_compute(coord_with_int)
    print_vertices_edges(vertices_final, edges_f)

#************************************************************
# main
#************************************************************

def main():
    while True:

        #******************************
        # get raw input and check cmd validity
        #******************************
        try:
            #cmd = input()
            cmd = sys.stdin.readline()
            if cmd == '':
                break

            #inp_lines.append(cmd)
            name = ''
            coords = ()
            
            #if cmd_a_chk.match(cmd) or cmd_m_chk.match(cmd) or cmd_r_chk.match(cmd):
                # record name and coordinates (if they exist)a
            # if cmd_a_chk.match(cmd) != 1:
            #     print("Error: Invalid input, please try again")
            if not cmd_g_chk.match(cmd):               
                name = re.findall(name_rx,cmd)[0]
                coords = [ tuple([ float(num) for num in re.findall(num_rx,coord) ]) \
                        for coord in re.findall(coord_rx,cmd)]
                
                name_upd = name.split('"')
                obj_street = Street(name_upd[1],coords)
            #Error handling, modify and deleting a street
            if not cmd_g_chk.match(cmd) and obj_street.name_check(): #str(name_upd[1]).isalpha() != 1 and str(name_upd[1]).isspace() != 1:
                print("Error: Street name is not in the right format, please input again")
            elif not cmd_r_chk.match(cmd) and not cmd_g_chk.match(cmd) and obj_street.coords_check() <=1 and cmd_a_chk.match(cmd) and cmd_m_chk.match(cmd) :
                print("Error: Input should have at least two coordinates, please try again")
            elif not cmd_g_chk.match(cmd) and name_upd[1][0] == " " :
                print("Error: Check for leading spaces and please try again")
            elif not cmd_g_chk.match(cmd) and name_space_chk.findall(name_upd[1]):
                print("Error: Check spaces in between street name and please try again")
            elif cmd_a_chk.match(cmd) and obj_street.street_search() != 1:
                #streets.append(name_upd[1])
                Street.street_storing(name_upd[1],coords)
                #obj_street.lines_compute()
            elif cmd_a_chk.match(cmd) and obj_street.street_search() == 1:
                print("Error: Street already exists, Please try again")
            elif (cmd_m_chk.match(cmd) or cmd_r_chk.match(cmd)) and obj_street.street_search() != 1:
                print("Error: Street doesn't exist, please try again")
            elif cmd_m_chk.match(cmd) and obj_street.street_search() == 1:
                modify_street(name_upd[1], coords)
            elif cmd_r_chk.match(cmd) and obj_street.street_search() == 1:
                delete_street(name_upd[1])
            elif cmd_g_chk.match(cmd):
                # print("in gg")
                # obj_street.lines_compute()
                #obj_street.lines_intersect()
                vertices_compute()
            else:
                print("Error: Please check for missing/additional quotes, brackets, spaces, commands etc.., and try again")
                      

        except EOFError:
            #break
            sys.exit(0)
        except IndexError:
            print("Error: Invalid input, please try again")

if __name__ == '__main__':
    main()