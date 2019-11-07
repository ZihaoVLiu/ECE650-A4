import re
import sys
from itertools import product, combinations

# get the cross point
def CPinSegment(p,line,line2):
    # detect whether the line is vertical
    if line[0][0] == line[1][0]:
        if  p[1] >= min(line[0][1],line[1][1]) and p[1] <= max(line[0][1],line[1][1]):
            if p[0] >= min(line2[0][0],line2[1][0]) and p[0] <= max(line2[0][0],line2[1][0]):
                return True
    # detect whether the line is horizontal
    elif line[0][1] == line[1][1]:
        if p[0] >= min(line[0][0],line[1][0]) and p[0] <= max(line[0][0],line[1][0]):
            if p[1] >= min(line2[0][1],line2[1][1]) and p[1] <= max(line2[0][1],line2[1][1]):
                return True
    else:
        if p[0] >= min(line[0][0],line[1][0]) and p[0] <= max(line[0][0],line[1][0]):
            if p[1] >= min(line2[0][1],line2[1][1]) and p[1] <= max(line2[0][1],line2[1][1]) and p[0] >= min(line2[0][0],line2[1][0]) and p[0] <= max(line2[0][0],line2[1][0]):
                return True
    return False

def getLinePara(line):
    a = line[0][1] - line[1][1]
    b = line[1][0] - line[0][0]
    c = line[0][0] * line[1][1] - line[1][0] * line[0][1]
    return a,b,c

def getCrossPoint(line1,line2):
    a1, b1, c1 = getLinePara(line1)
    a2, b2, c2 = getLinePara(line2)
    d = a1 * b2 - a2 * b1
    p = [0,0]
    if d == 0: # line1 and line2 are horizontal
        return ()
    else:
        p[0] = float((b1 * c2 - b2 * c1)*1.0 / d)
        p[1] = float((c1 * a2 - c2 * a1)*1.0 / d)
    if CPinSegment(p,line1,line2):
        return p
    else:
        return ()

def getCombination(list1,list2):
    combinationList = []
    for item in product(list1,list2):
        combinationList.append(item)
    return combinationList

def removeDuplicates(origin_list,target_line):
    for id in origin_list:
        if id not in target_line:
            target_line.append(id)

def capitalDetect(input, list):
    for name in list:
        if input.upper() == name.upper():
            print("Error: The program is case insensitive. The street name has been input.")

def crossPointInSegment(p,line):
    # if p is on line, then return the line coordinates, if not, return 0.
    if line[0][0]-line[1][0] != 0:
        k = (float(line[0][1]-line[1][1])/float(line[0][0]-line[1][0]))
        b = float(line[0][1]-k*line[0][0])
        if abs(p[1] - (k*p[0]+b)) < 0.0001 and p[0] >= min(line[0][0],line[1][0]) and p[0] <= max(line[0][0],line[1][0]) and p[1] >= min(line[0][1],line[1][1]) and p[1] <= max(line[0][1],line[1][1]):
            return line[0],line[1]
        else:
            return 0
    else:
        if p[0]==line[0][0] and p[1] >= min(line[0][1], line[1][1]) and p[1] <= max(line[0][1], line[1][1]):
            return line[0],line[1]
        else:
            return 0
def segInSeg(line1,line2):
    if crossPointInSegment(line1[0],line2) and crossPointInSegment(line1[1],line2):
        return True
    else:
        return False

def cloneList(li1):
    li_copy = li1[:]
    return li_copy


def main():
    try:
        information = []
        streetNameList = []
        dictStreet = {}
        while True:
            #you may change here
            #sys.stdout.flush()
            gather = sys.stdin.readline()
            # use regular expression to get street name.
            patternStreet = re.compile('"(.*)"')
            streetName = patternStreet.findall(gather)
            # use regular expression to get street coordinate.
            patternCoord = re.compile('\-\d+|\d+')
            streetCoord = patternCoord.findall(gather)


            if gather == "quit":
                break
            elif gather[0] not in ["a", "c", "r", "g"]:
                print("Error: Command is invalid. Please try again.")
                continue
            elif gather[0] in ["a", "c", "r"] and gather[1] != " ":
                print("Error: Your input command format is not correct (Need space behind the command). Please input again.")
                continue
            # detect whether a space between '"' and '('
            elif (gather[0] == "a" or gather[0] == "c") and (gather[gather.index("(")-1] != " " or ("(" not in gather or ")" not in gather)):
                print("Error: Command is invalid. Please try again. ")
                continue
            elif (gather[0] == "a" or gather[0] == "c") and (len(streetName) ==0 or len(streetCoord) < 4 or len(streetCoord) % 2 != 0):
                print("Error: No street name or insufficient street coordinates are input. Please try again.")
                continue


            # change the list from string to int.
            i = 0
            for coord in streetCoord:
                streetCoord[i] = float(coord)
                i += 1

            # perform the add command
            if gather[0] == "a":
                if gather.count("(") + gather.count(")") != len(streetCoord):
                    print("Error: Expected correct parentheses input.")
                    continue
                # detect whether the street name is capital different.
                judge = True
                capitalDetect(streetName[0], streetNameList)
                for name in streetNameList:
                    if streetName[0].upper() == name.upper():
                        dictStreet[name] = streetCoord
                        judge = False
                # avoid duplicate input
                if (streetName[0] not in streetNameList) or judge:
                    # put key and value into the dictionary
                    dictStreet[streetName[0]] = streetCoord
                    # put the street name into a list
                    streetNameList.append(streetName[0])
                    information.append(gather)
                else:
                    print("Error: Input street has already inputted")
                    continue
            # perform the change command
            elif gather[0] == "c":
                judge = False
                if gather.count("(") + gather.count(")") != len(streetCoord):
                    print("Error: Expected correct parentheses input.")
                    continue
                for name in streetNameList:
                    if streetName[0].upper() == name.upper():
                        dictStreet[name] = streetCoord
                        judge = True
                if streetName[0] in streetNameList or judge:
                    dictStreet[streetName[0]] = streetCoord
                else:
                    print("Error: Input street does not exist.")
                    continue
            # perform the remove command
            elif gather[0] == "r":
                if gather.count("(") + gather.count(")") != len(streetCoord):
                    print("Error: Expected correct parentheses input.")
                    continue
                for name in streetNameList:
                    if streetName[0].upper() == name.upper():
                        dictStreet[name] = streetCoord
                        judge = False
                        namedel = name
                if streetName[0] not in streetNameList and judge:
                    print("Error: Input street does not exist.")
                    continue
                else:
                    dictStreet.pop(namedel)
                    streetNameList.remove(namedel)
            # perform the graph command
            elif gather[0] == "g":
                # construct a len()-dimension list
                lineSegCoord = [[0 for col in range(1)] for row in range(len(streetNameList))]
                for i in range(len(streetNameList)):
                    del lineSegCoord[i][0]
                # put coordinate of each street into len()-D list
                linej = 0
                for name in streetNameList:
                    lineCoord = dictStreet[name]
                    j=2
                    while j <= len(lineCoord):
                        lineSegCoord[linej].append(lineCoord[(j-2):j])
                        j = j+2
                    linej += 1
                # construct a dictionary to store street segment of each street
                dictSeg = {}
                for nameNum in range(len(streetNameList)):
                    coordListTemp = []
                    for seq in range(len(lineSegCoord[nameNum])-1):
                        coordListTemp.append(lineSegCoord[nameNum][seq:seq+2])
                        dictSeg[streetNameList[nameNum]] = coordListTemp
                # output the street combination in a list
                streetNameCombList = list(combinations(streetNameList,2))

                # build lists to store cross point and end point
                crossPointList = []
                endPointList = []
                # build a dictionary to store all relative endpoints
                dictCE={}
                # get the cross point
                for comb in streetNameCombList:
                    streetLineComb = getCombination(dictSeg[comb[0]],dictSeg[comb[1]])
                    for segs in streetLineComb:
                        crossPoint = tuple(getCrossPoint(segs[0],segs[1]))
                        crossPointList.append(crossPoint)
                        # store each end point into a list
                        if crossPoint != ():
                            endPointList.append(tuple(segs[0][0]))
                            endPointList.append(tuple(segs[0][1]))
                            endPointList.append(tuple(segs[1][0]))
                            endPointList.append(tuple(segs[1][1]))
                # remove the duplicate values and None values
                endPointListU = []
                removeDuplicates(endPointList, endPointListU)
                crossPointListU = []
                removeDuplicates(crossPointList, crossPointListU)
                if () in crossPointListU:
                    crossPointListU.remove(())
                # build a list to store vertex
                vertex = []
                removeDuplicates(crossPointListU + endPointListU, vertex)
                # build a dict to store vertex
                dictVertex = {}
                for dictV in range(len(vertex)):
                    dictVertex[str(dictV+1)]=vertex[dictV]
                # print the vertex of g command
                sys.stdout.write("V ")
                length = str(len(dictVertex))
                sys.stdout.write(length+"\n")
                sys.stdout.flush()

                # use dictionary to indicate all vertex according to the cross point
                # get a combination of all endpoints
                vertexCombSegList = getCombination(endPointListU, endPointListU)
                dictC2V={}
                for cp in crossPointListU:
                    listC2V = []
                    for eps in vertexCombSegList:
                        if crossPointInSegment(cp, eps) != 0:
                            listC2V.append(crossPointInSegment(cp, eps)[0])
                            listC2V.append(crossPointInSegment(cp, eps)[1])
                    listC2VU=[]
                    # remove all duplicate points
                    removeDuplicates(listC2V, listC2VU)
                    dictC2V[cp] = tuple(listC2VU)


                # output the cross point dictionary indicating edge
                # remove edge including other vertex
                for cp in crossPointListU:
                    edgeWithOVlist = [] # edge with other vertexs
                    temp = list(dictC2V[cp])
                    for vp in dictC2V[cp]:
                        edge=[cp,vp]
                        crossPointListUDup = cloneList(crossPointListU)
                        crossPointListUDup.remove(cp) # remove the crosspoint of edge itself
                        for p in crossPointListUDup:
                            if crossPointInSegment(p, edge) != 0:
                                if crossPointInSegment(p, edge)[0] in temp:
                                    temp.remove(crossPointInSegment(p, edge)[0]) # remove the crosspoint vertex that between edge
                                    dictC2V[cp] = tuple(temp)
                                if crossPointInSegment(p, edge)[1] in temp:
                                    temp.remove(crossPointInSegment(p, edge)[1])
                                    dictC2V[cp] = tuple(temp)

                # delete the endpoint of edge that does not exist
                for keys in dictC2V:
                    for values in dictC2V[keys]:
                        test = []
                        test.append(keys)
                        test.append(values)
                        TFvalues = 0
                        segValues = dictSeg.values()
                        for firstV in segValues:
                            for secondV in firstV:
                                if segInSeg(test, secondV):
                                    TFvalues = 1
                                    continue
                        if TFvalues == 0:
                            listC2Vdel = list(dictC2V[keys])
                            listC2Vdel.remove(values)
                            dictC2V[keys] = tuple(listC2Vdel)

                # print the edge and remove the duplicate edge
                listEdge=[]
                tupleEdge=()
                for cp in crossPointListU:
                    for cpD in dictVertex:
                        if dictVertex[cpD] == cp:
                            break
                    for edge in dictC2V[cp]:
                        for vertex in dictVertex:
                            if dictVertex[vertex] == edge:
                                tupleEdge = (int(vertex), int(cpD))
                                listEdge.append(tupleEdge)

                # get a combination of all crosspoint and test it and input it into listEdge
                crossCombSegList = list(combinations(crossPointListU, 2))
                dictC2E = {}
                for ep in endPointListU:
                    listC2E = []
                    for cps in crossCombSegList:
                        crossPointListU2nd = cloneList(crossPointListU)
                        judge_point = 0
                        if crossPointInSegment(ep, cps) != 0:
                            listC2E.append(crossPointInSegment(ep, cps)[0])
                            listC2E.append(crossPointInSegment(ep, cps)[1])
                        listC2EU = []
                        # remove all duplicate points
                        removeDuplicates(listC2E, listC2EU)
                        # detect whether cps on edges.
                        TFvalues = False
                        segValues = dictSeg.values()
                        for firstV in segValues:
                            for secondV in firstV:
                                if segInSeg(cps,secondV):
                                    TFvalues = segInSeg(cps,secondV)
                        if len(listC2EU) == 0 and TFvalues:
                            for vertex1 in dictVertex:
                                if dictVertex[vertex1] == cps[0]:
                                    break
                            for vertex2 in dictVertex:
                                if dictVertex[vertex2] == cps[1]:
                                    break
                            crossPointListU2nd.remove(dictVertex[vertex1])
                            crossPointListU2nd.remove(dictVertex[vertex2])
                            for point in crossPointListU2nd:
                                if crossPointInSegment(point, [dictVertex[vertex1],dictVertex[vertex2]]) != 0:
                                    judge_point = 1
                            if judge_point == 0:
                                tupleEdge = (int(vertex1), int(vertex2))
                                listEdge.append(tupleEdge)

                # delete bad edges
                listDupEdge = []
                listEdgeD = []
                # delete the duplicate edge
                for edge in listEdge:
                    if edge not in listEdgeD:
                        listEdgeD.append(edge)
                # delete the edge of same point
                for edge in listEdgeD:
                    if edge[0] == edge[1]:
                        listEdgeD.remove(edge)
                # delete duplicate edges
                for edge1 in listEdgeD:
                    for edge2 in listEdgeD:
                        if edge1[0] == edge2[1] and edge1[1] == edge2[0]:
                            listDupEdge.append(edge1)
                for dup in listDupEdge:
                    listEdgeD.remove(dup)

                # output the edge in "<>" format
                sys.stdout.write("E {")
                for edge in range(len(listEdgeD)-1):
                    sys.stdout.write("<%d,%d>," %(listEdgeD[edge][0]-1, listEdgeD[edge][1]-1)) # minus 1 to satisfy a2.
                if len(listEdgeD)!= 0:
                    sys.stdout.write("<%d,%d>" %(listEdgeD[len(listEdgeD)-1][0]-1, listEdgeD[len(listEdgeD)-1][1]-1)) # minus 1 to satisfy a2.
                sys.stdout.write("}\n")
                sys.stdout.flush()
    except EOFError:
        sys.exit()

if __name__ == '__main__':
    main()