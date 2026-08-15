#ifndef _PHYSICS_H_
#define _PHYSICS_H_



typedef struct  core_t core_t;


typedef struct physics_t
{
    int ax;
    int ay;


}physics_t;



#ifdef __cplusplus
extern "C" {

    //void physics_update_core(core_t *core);
    void physics_update(core_t *core, physics_t *physics);

#endif


#ifdef __cplusplus
}
#endif

#endif