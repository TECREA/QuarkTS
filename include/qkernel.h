/*!
 * @file qkernel.h
 * @author J. Camilo Gomez C.
 * @version 3.27
 * @note This file is part of the QuarkTS distribution.
 * @brief Kernel API interface to create/remove tasks and perform special OS operations.
 **/
#ifndef QKERNEL_H
    #define QKERNEL_H

    #include "qtypes.h"
    #include "qcritical.h"
    #include "qtasks.h"

    #if ( Q_ATCLI == 1)
        #include "qatcli.h"
    #endif
   
    #ifdef __cplusplus
    extern "C" {
    #endif

    #ifndef Q_PRIORITY_LEVELS
        #define Q_PRIORITY_LEVELS           ( 3 )
    #endif
    
    #ifndef Q_SETUP_TIME_CANONICAL
        #define Q_SETUP_TIME_CANONICAL      ( 0 )
    #endif
    
    #ifndef Q_SETUP_TICK_IN_HERTZ
        #define Q_SETUP_TICK_IN_HERTZ       ( 0 )
    #endif
    
    #ifndef Q_PRIO_QUEUE_SIZE
        #define Q_PRIO_QUEUE_SIZE           ( 3 )
    #endif

    #ifndef Q_PRIO_QUEUE_SIZE
        #define Q_PRIO_QUEUE_SIZE           ( 3 )
    #endif

    #ifndef Q_PRESERVE_TASK_ENTRY_ORDER
        #define Q_PRIO_QUEUE_SIZE           ( 0 )
    #endif

    #ifndef Q_NOTIFICATION_SPREADER
        #define Q_NOTIFICATION_SPREADER     ( 1 )
    #endif

    #ifndef Q_ALLOW_SCHEDULER_RELEASE
        #define Q_ALLOW_SCHEDULER_RELEASE   ( 1 )
    #endif

    #if ( Q_PRIORITY_LEVELS < 1 )
        #error Q_PRIORITY_LEVELS must be defined to be greater than or equal to 1.
    #endif    

    /** @addtogroup qtaskcreation
     * @brief Kernel API interface to create/remove tasks and perform special OS operations.
     *  @{
     */

    /*Default priority levels*/

    /** @brief A macro directive to indicate the lowest priority level. */
    #define qLowest_Priority        ( (qPriority_t)( 0u ) )

    /** @brief A macro directive to indicate the medium priority level. */
    #define qMedium_Priority        ( (qPriority_t)( Q_PRIORITY_LEVELS >> 1 ) )

    /** @brief A macro directive to indicate the highest priority level. */
    #define qHigh_Priority          ( (qPriority_t)( Q_PRIORITY_LEVELS - 1u ) )

    /** @brief A directive indicating that the task will run periodically every time its time has expired. */
    #define qPeriodic               ( (qIteration_t)(-2147483647-1) )

    /** @brief Same as #qPeriodic. A directive indicating that the task will run periodically every time its time has expired. */
    #define qIndefinite             ( qPeriodic )

    /** @brief A directive that indicates that the task will be executed only once after its time has expired.*/
    #define qSingleShot             ( (qIteration_t)(1) )

    #if (Q_SETUP_TIME_CANONICAL == 1)
        void qOS_Setup( const qGetTickFcn_t TickProvider, qTaskFcn_t IdleCallback );
    #else
        /**
        * @brief Task Scheduler Setup. This function is required and must be called once in 
        * the application main thread before any task is being added to the OS.
        * @param TickProvider The function that provides the tick value. If the user application 
        * uses the <b>qClock_SysTick()</b> from the ISR, this parameter can be NULL.
        * @note Function should take void and return a 32bit value. 
        * @param BaseTimming (Optional) This parameter specifies the ISR background timer base time.
        * This can be the period in seconds(Floating-point format) or frequency 
        * in Herzt(Only if Q_SETUP_TICK_IN_HERTZ is enabled).
        * @param IdleCallback  Callback function to the Idle Task. To disable the 
        * Idle Task activities, pass NULL as argument.
        * @return none.
        */      
        void qOS_Setup( const qGetTickFcn_t TickProvider, const qTimingBase_t BaseTimming, qTaskFcn_t IdleCallback );
    #endif

    /**
    * @brief Set/Change the callback for the Idle-task
    * @param Callback A pointer to a void callback method with a qEvent_t parameter 
    * as input argument. To disable pass NULL as argument.
    * @return none.
    */
    void qOS_Set_IdleTask( qTaskFcn_t Callback );

    #if ( Q_ALLOW_SCHEDULER_RELEASE == 1 )
        /**
        * @brief Disables the kernel scheduling. The main thread will continue after the
        * <b>qOS_Run()</b> call.
        * @return none.
        */          
        void qOS_Scheduler_Release( void );

        /**
        * @brief Set/Change the scheduler release callback function
        * @param Callback A pointer to a void callback method with a qEvent_t parameter 
        * as input argument.
        * @return none.
        */     
        void qOS_Set_SchedulerReleaseCallback( qTaskFcn_t Callback );
    #endif       

    /** @}*/

    /** @addtogroup qnot
    *  @{
    */

    /**
    * @brief Try to spread a notification among all the tasks in the scheduling scheme
    * @note Operation will be performed in the next scheduling cycle. 
    * @param eventdata Specific event user-data.
    * @param mode the method used to spread the event: ::qTask_NotifySimple or ::qTask_NotifyQueued.
    * @return #qTrue if success. #qFalse if any other spread operation is in progress.
    */
    qBool_t qOS_Notification_Spread( void *eventdata, const qTask_NotifyMode_t mode );
    /** @}*/

    /** @addtogroup qtaskcreation
     * @brief Kernel API interface to create/remove tasks and perform special OS operations.
     *  @{
     */

    /**
    * @brief Add a task to the scheduling scheme. The task is scheduled to run every <b>Time</b> 
    * seconds, <b>nExecutions</b> times and executing <b>CallbackFcn</b> method on every pass.
    * @param Task  A pointer to the task node.
    * @param CallbackFcn A pointer to a void callback method with a qEvent_t parameter 
    * as input argument.
    * @param Priority Task priority Value. [0(min) - Q_PRIORITY_LEVELS(max)]
    * @param Time Execution interval defined in seconds (floating-point format). 
    * For immediate execution (tValue = #qTimeImmediate).
    * @param nExecutions Number of task executions (Integer value). For indefinite 
    * execution (nExecutions = #qPeriodic or #qIndefinite). Tasks do not 
    * remember the number of iteration set initially. After the 
    * iterations are done, internal iteration counter is 0. To perform 
    * another set of iterations, set the number of iterations again.
    * @note Tasks which performed all their iterations put their own state to #qDisabled.
    * @note Asynchronous triggers do not affect the iteration counter.
    * @param InitialState Specifies the initial operational state of the task 
    * (#qEnabled, #qDisabled, #qAsleep or #qAwake(implies #qEnabled)).
    * @param arg Represents the task arguments. All arguments must be passed by
    * reference and cast to (void *). Only one argument is allowed, 
    * so, for multiple arguments, create a structure that contains 
    * all of the arguments and pass a pointer to that structure.
    * @return Returns #qTrue on success, otherwise returns #qFalse.
    */    
    qBool_t qOS_Add_Task( qTask_t * const Task, qTaskFcn_t CallbackFcn, qPriority_t Priority, qTime_t Time, qIteration_t nExecutions, qState_t InitialState, void* arg );

    /**
    * @brief Add a task to the scheduling scheme.  This API creates a task with qDisabled 
    * state by default , so this task will be oriented to be executed only, when 
    * asynchronous events occurs. However, this behavior can be changed in execution
    * time using <b>qTask_Set_Time()</b> or <b>qTask_Set_Iterations()</b>.
    * @param Task  A pointer to the task node.
    * @param CallbackFcn A pointer to a void callback method with a qEvent_t parameter 
    * as input argument.
    * @param Priority Task priority Value. [0(min) - Q_PRIORITY_LEVELS(max)]
    * @param arg Represents the task arguments. All arguments must be passed by
    * reference and cast to (void *). Only one argument is allowed, 
    * so, for multiple arguments, create a structure that contains 
    * all of the arguments and pass a pointer to that structure.
    * @return Returns #qTrue on success, otherwise returns #qFalse.
    */       
    qBool_t qOS_Add_EventTask( qTask_t * const Task, qTaskFcn_t CallbackFcn, qPriority_t Priority, void* arg );
    #if ( Q_FSM == 1)
        /**
        * @brief Add a task to the scheduling scheme running a dedicated state-machine. 
        * The task is scheduled to run every <b>Time</b> seconds in qPeriodic mode. The event info
        * will be available as a generic pointer inside the <b>Data</b> field of the qSM_Handler_t argument.
        * @param Task  A pointer to the task node.
        * @param m  A pointer to the Finite State-Machine (FSM) object.    
        * @param Priority Task priority Value. [0(min) - Q_PRIORITY_LEVELS(max)]
        * @param Time Execution interval defined in seconds (floating-point format). 
        * For immediate execution (tValue = #qTimeImmediate).
        * @param InitialTaskState Specifies the initial operational state of the task 
        * (#qEnabled, #qDisabled, #qAsleep or #qAwake(implies #qEnabled)).
        * @param arg Represents the task arguments. All arguments must be passed by
        * reference and cast to (void *). Only one argument is allowed, 
        * so, for multiple arguments, create a structure that contains 
        * all of the arguments and pass a pointer to that structure.
        * @return Returns #qTrue on success, otherwise returns #qFalse.
        */     
        qBool_t qOS_Add_StateMachineTask( qTask_t * const Task, qSM_t *m, qPriority_t Priority, qTime_t Time, qState_t InitialTaskState, void *arg );                  
    #endif

    #if ( Q_ATCLI == 1)
        /**
        * @brief Add a task to the scheduling scheme running an AT Command Parser. Task will be scheduled
        * as an event-triggered task. The parser address will be stored in the TaskData storage-Pointer.
        * @param Task A pointer to the task node.
        * @param cli A pointer to the AT Command Line Inteface instance.
        * @param Priority Task priority Value. [0(min) - Q_PRIORITY_LEVELS(max)]
        * @return Returns #qTrue on success, otherwise returns #qFalse.
        */       
        qBool_t qOS_Add_ATCLITask( qTask_t * const Task, qATCLI_t *cli, qPriority_t Priority );
    #endif

    /**
    * @brief Remove the task from the scheduling scheme.
    * @param Task A pointer to the task node.
    * @return Returns #qTrue if success, otherwise returns #qFalse.
    */  
    qBool_t qOS_Remove_Task( qTask_t * const Task );

    /**
    * @brief Executes the scheduling scheme. It must be called once after the task
    * pool has been defined.
    * @note This call keeps the application in an endless loop.
    * @return none.
    */      
    void qOS_Run( void );

    /** @}*/

    #ifdef __cplusplus
    }
    #endif

#endif /*QKERNEL_H*/
