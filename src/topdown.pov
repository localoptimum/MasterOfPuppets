#version 3.7;
#include "colors.inc"
#include "stones.inc"
global_settings { assumed_gamma 1 
   ambient_light rgb<1,1,1> }
camera {
   location  <-10,80,-1>
   look_at   <-10,0,20>
   angle 60.0 }
background {color rgb <0.4, 0.4, 0.4>}
light_source {<-30, 20, 20>, rgb <1,1,1>}
light_source {<-5, 20, 40>, rgb <1,1,1>}
box { <-45, -2, 55> , <0, -1, -1> texture {T_Stone16 scale 0.5 } }

#include "best_layout.pov"
