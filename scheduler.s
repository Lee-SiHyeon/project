	.syntax unified
	.thumb

	.text

	.align 4

	.extern current_tcb		//OS.c
	.extern next_tcb		//OS.c
	.extern p_current_tcb   //OS.c
	.extern _OS_Scheduler //OS.c
	.global PendSV_Handler
	.type 	PendSV_Handler, %function
PendSV_Handler:

	@ critical section context switch 구현 해야함.
    // 현재 태스크의 컨텍스트를 저장
	MRS     R0, PSP                         // PSP를 R0에 가져와서
    STMDB   R0!, {R4-R11}                   // R4-R11 레지스터를 PSP에 저장
    LDR     R1, =current_tcb               // current_tcb 주소를 R1에 로드
    LDR     R1, [R1]                        // current_tcb 값을 R1에 로드
    STR     R0, [R1]                        // 업데이트된 스택 포인터를 현재 TCB에 저장

    // 다음 실행할 태스크를 결정
    PUSH    {r0,LR}                            // LR을 스택에 저장
	BL 		_OS_Scheduler					// call _OS_scheduler
    POP     {r0,LR}                            // LR을 스택에서 복원

    // 다음 태스크의 컨텍스트를 복원
    LDR     R1, =next_tcb               // next_tcb 주소를 R1에 로드 @impl next_tcb 선언 및 사용해야함.
    LDR     R1, [R1]                        // next_tcb 값을 R1에 로드
    LDR     R0, [R1]                        // 새로운 스택 포인터를 R0에 로드
    LDMFD   R0!, {R4-R11}                   // R4-R11 레지스터를 스택에서 복원
    MSR     PSP, R0                         // 프로세스 스택 포인터를 업데이트

	// restore 이후 current_tcb=next_tcb가 되어야함.
	LDR		R0, =p_current_tcb
	LDR		R1, =next_tcb
	LDR     R0, [R0]
	LDR     R1, [R1]

	STR     R1, [R0]

    // 인터럽트 종료
	ORR 	LR, LR, #0xd
    BX      LR                              // 인터럽트 종료

	.global SVC_Handler
	.type 	SVC_Handler, %function
SVC_Handler:
	LDR		R3, =current_tcb
	LDR		R1, [R3] @ current_tcb 가져와서
	LDR		R0, [R1] @ current_tcb 의 시작주소 ==> top_of_stack를 가져와서
	LDMIA	R0!, {R4-R11} @ top_of_state
	MSR		PSP, R0

	ORR		LR, LR, #0xd
	BX		LR

	.global _OS_Start_First_Task
	.type 	_OS_Start_First_Task, %function
_OS_Start_First_Task:
	
	SVC #0
	BX LR
