class mapTest{
   static void main(String[] args){

   def myMap = [
   'name' : 'jason',
   'age' : 25,
   'address' : 'yuhua st',
   'list' : [1,2,3,4],
    123   :  456
   ]

   println("Name " + myMap['name'])
   println("Age " + myMap.get('age'))
   println("List Item " + myMap['list'][0..2])
   myMap.put('city','NJ')
   println("Has phone " + myMap.containsKey('phone'))
   println("Size " + myMap.size())

   def keys = myMap.keySet();
   println("myMap has keys " + keys)

   for(key in keys)
   {
      println("myMap [key:" + key + ",value:" + myMap[key] + "]")
   }

   for( item in myMap)
   {
     println("$item.key : $item.value")
   }

   println("myMap is $myMap")
  }
}
