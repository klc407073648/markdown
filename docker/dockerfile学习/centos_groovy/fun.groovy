class listTest{
   static void main(String[] args){
   
   def name = 'Jason'
   passByValue(name)
   println(name)
  
   def listToDouble = [1,2,3,4]
   listToDouble=doubleList(listToDouble);
   println('listToDouble: ' + listToDouble)
   //def listToTriple = [1,2,3]
   //triList(listToTriple)
   // println(listToTriple)

   def nums =sumAll(1,2,3,4,5)
   println('Sum: ' + nums)

   def greeting = "Goodbye"
   def sayGoodbye = {theName ->
   println("$greeting $theName")}

   sayGoodbye("Hellen")

   def numList = [1,2,3,4,5]
   numList.each { println(it) }

   def employees = [
   'Paul' : 34,
   'Sally' : 35,
   'Ken'  : 36 
   ];

   employees.each { println("$it.key : $it.value") }

   def nameList = [ 'Lucy' , 'Canna' , 'Joh']
   def matchEleOne = nameList.find { item -> item == 'Joh'}
   def matchEleTwo = nameList.find { item -> item == 'Jason'}
   
   println(matchEleOne)
   println(matchEleTwo)

   def randNumList = [1,2,3,4,5,6,7,8]
   def numMatches = randNumList.findAll { item ->  item >4 && item <7}
   println(numMatches)

   println('>5 :' + randNumList.any { item -> item >5 } )
   println('>1 :' + randNumList.every { item -> item >1 } )
   println('Double :' + randNumList.collect { item -> item * 2 } )

   def getEven = { num -> return(num%2 == 0)}
   def evenNums = listEdit(randNumList,getEven)
   println("Evens : " + evenNums)
 }

   static def passByValue(name)
   {
      name = 'In passByValue'
      println(name)
   }

   static def doubleList(list)
   {
      def newList = list.collect{ it*2 }
      return newList;
   }

   static def triList(list)
   {
      list = list.collect{ it*3 }
      println(list)
     // must return value
   }
   
   static def sumAll(int... num)
   {
      def sum = 0;
      num.each { sum += it}
      return sum;
   }
   
   static def listEdit(list,clo)
   {
     return list.findAll(clo)
   }
}

