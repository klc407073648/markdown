class listTest{
   static void main(String[] args){

   def oneTo10 = 1..10;
   def threeTo9 = 3..<10;
   def aToZ = 'a'..'z';

   println('oneTo10: ' + oneTo10)
   println('threeTo9: ' + threeTo9)
   println('aToZ: ' + aToZ)

   println('oneTo10 Test:' )
   println('Size ' + oneTo10.size())
   println('2nd Item of oneTo10 ' + oneTo10.get(1))
   println('contains 11 ' + oneTo10.contains(11))

   println('Get First ' + oneTo10.getFrom())
   println('Get Last ' + oneTo10.getTo())

   println('threeTo10 Test:' )
   println('Get First ' + threeTo9.getFrom())
   println('Get Last ' + threeTo9.getTo())
   

   println('aToZ Test:' )
   println('2nd ' + aToZ.get(1))
   println('1-4th aToZ ' + aToZ.subList(1,4)); 
   
  }
}

