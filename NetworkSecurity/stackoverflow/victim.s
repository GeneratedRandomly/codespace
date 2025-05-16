
victim:     file format elf32-i386


Disassembly of section .init:

08049000 <_init>:
 8049000:	f3 0f 1e fb          	endbr32 
 8049004:	53                   	push   %ebx
 8049005:	83 ec 08             	sub    $0x8,%esp
 8049008:	e8 c3 00 00 00       	call   80490d0 <__x86.get_pc_thunk.bx>
 804900d:	81 c3 f3 2f 00 00    	add    $0x2ff3,%ebx
 8049013:	8b 83 fc ff ff ff    	mov    -0x4(%ebx),%eax
 8049019:	85 c0                	test   %eax,%eax
 804901b:	74 02                	je     804901f <_init+0x1f>
 804901d:	ff d0                	call   *%eax
 804901f:	83 c4 08             	add    $0x8,%esp
 8049022:	5b                   	pop    %ebx
 8049023:	c3                   	ret    

Disassembly of section .plt:

08049030 <__libc_start_main@plt-0x10>:
 8049030:	ff 35 04 c0 04 08    	push   0x804c004
 8049036:	ff 25 08 c0 04 08    	jmp    *0x804c008
 804903c:	00 00                	add    %al,(%eax)
	...

08049040 <__libc_start_main@plt>:
 8049040:	ff 25 0c c0 04 08    	jmp    *0x804c00c
 8049046:	68 00 00 00 00       	push   $0x0
 804904b:	e9 e0 ff ff ff       	jmp    8049030 <_init+0x30>

08049050 <printf@plt>:
 8049050:	ff 25 10 c0 04 08    	jmp    *0x804c010
 8049056:	68 08 00 00 00       	push   $0x8
 804905b:	e9 d0 ff ff ff       	jmp    8049030 <_init+0x30>

08049060 <gets@plt>:
 8049060:	ff 25 14 c0 04 08    	jmp    *0x804c014
 8049066:	68 10 00 00 00       	push   $0x10
 804906b:	e9 c0 ff ff ff       	jmp    8049030 <_init+0x30>

08049070 <puts@plt>:
 8049070:	ff 25 18 c0 04 08    	jmp    *0x804c018
 8049076:	68 18 00 00 00       	push   $0x18
 804907b:	e9 b0 ff ff ff       	jmp    8049030 <_init+0x30>

Disassembly of section .text:

08049080 <_start>:
 8049080:	f3 0f 1e fb          	endbr32 
 8049084:	31 ed                	xor    %ebp,%ebp
 8049086:	5e                   	pop    %esi
 8049087:	89 e1                	mov    %esp,%ecx
 8049089:	83 e4 f0             	and    $0xfffffff0,%esp
 804908c:	50                   	push   %eax
 804908d:	54                   	push   %esp
 804908e:	52                   	push   %edx
 804908f:	e8 19 00 00 00       	call   80490ad <_start+0x2d>
 8049094:	81 c3 6c 2f 00 00    	add    $0x2f6c,%ebx
 804909a:	6a 00                	push   $0x0
 804909c:	6a 00                	push   $0x0
 804909e:	51                   	push   %ecx
 804909f:	56                   	push   %esi
 80490a0:	c7 c0 21 92 04 08    	mov    $0x8049221,%eax
 80490a6:	50                   	push   %eax
 80490a7:	e8 94 ff ff ff       	call   8049040 <__libc_start_main@plt>
 80490ac:	f4                   	hlt    
 80490ad:	8b 1c 24             	mov    (%esp),%ebx
 80490b0:	c3                   	ret    
 80490b1:	66 90                	xchg   %ax,%ax
 80490b3:	66 90                	xchg   %ax,%ax
 80490b5:	66 90                	xchg   %ax,%ax
 80490b7:	66 90                	xchg   %ax,%ax
 80490b9:	66 90                	xchg   %ax,%ax
 80490bb:	66 90                	xchg   %ax,%ax
 80490bd:	66 90                	xchg   %ax,%ax
 80490bf:	90                   	nop

080490c0 <_dl_relocate_static_pie>:
 80490c0:	f3 0f 1e fb          	endbr32 
 80490c4:	c3                   	ret    
 80490c5:	66 90                	xchg   %ax,%ax
 80490c7:	66 90                	xchg   %ax,%ax
 80490c9:	66 90                	xchg   %ax,%ax
 80490cb:	66 90                	xchg   %ax,%ax
 80490cd:	66 90                	xchg   %ax,%ax
 80490cf:	90                   	nop

080490d0 <__x86.get_pc_thunk.bx>:
 80490d0:	8b 1c 24             	mov    (%esp),%ebx
 80490d3:	c3                   	ret    
 80490d4:	66 90                	xchg   %ax,%ax
 80490d6:	66 90                	xchg   %ax,%ax
 80490d8:	66 90                	xchg   %ax,%ax
 80490da:	66 90                	xchg   %ax,%ax
 80490dc:	66 90                	xchg   %ax,%ax
 80490de:	66 90                	xchg   %ax,%ax

080490e0 <deregister_tm_clones>:
 80490e0:	b8 24 c0 04 08       	mov    $0x804c024,%eax
 80490e5:	3d 24 c0 04 08       	cmp    $0x804c024,%eax
 80490ea:	74 24                	je     8049110 <deregister_tm_clones+0x30>
 80490ec:	b8 00 00 00 00       	mov    $0x0,%eax
 80490f1:	85 c0                	test   %eax,%eax
 80490f3:	74 1b                	je     8049110 <deregister_tm_clones+0x30>
 80490f5:	55                   	push   %ebp
 80490f6:	89 e5                	mov    %esp,%ebp
 80490f8:	83 ec 14             	sub    $0x14,%esp
 80490fb:	68 24 c0 04 08       	push   $0x804c024
 8049100:	ff d0                	call   *%eax
 8049102:	83 c4 10             	add    $0x10,%esp
 8049105:	c9                   	leave  
 8049106:	c3                   	ret    
 8049107:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 804910e:	66 90                	xchg   %ax,%ax
 8049110:	c3                   	ret    
 8049111:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 8049118:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 804911f:	90                   	nop

08049120 <register_tm_clones>:
 8049120:	b8 24 c0 04 08       	mov    $0x804c024,%eax
 8049125:	2d 24 c0 04 08       	sub    $0x804c024,%eax
 804912a:	89 c2                	mov    %eax,%edx
 804912c:	c1 e8 1f             	shr    $0x1f,%eax
 804912f:	c1 fa 02             	sar    $0x2,%edx
 8049132:	01 d0                	add    %edx,%eax
 8049134:	d1 f8                	sar    %eax
 8049136:	74 20                	je     8049158 <register_tm_clones+0x38>
 8049138:	ba 00 00 00 00       	mov    $0x0,%edx
 804913d:	85 d2                	test   %edx,%edx
 804913f:	74 17                	je     8049158 <register_tm_clones+0x38>
 8049141:	55                   	push   %ebp
 8049142:	89 e5                	mov    %esp,%ebp
 8049144:	83 ec 10             	sub    $0x10,%esp
 8049147:	50                   	push   %eax
 8049148:	68 24 c0 04 08       	push   $0x804c024
 804914d:	ff d2                	call   *%edx
 804914f:	83 c4 10             	add    $0x10,%esp
 8049152:	c9                   	leave  
 8049153:	c3                   	ret    
 8049154:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 8049158:	c3                   	ret    
 8049159:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi

08049160 <__do_global_dtors_aux>:
 8049160:	f3 0f 1e fb          	endbr32 
 8049164:	80 3d 24 c0 04 08 00 	cmpb   $0x0,0x804c024
 804916b:	75 1b                	jne    8049188 <__do_global_dtors_aux+0x28>
 804916d:	55                   	push   %ebp
 804916e:	89 e5                	mov    %esp,%ebp
 8049170:	83 ec 08             	sub    $0x8,%esp
 8049173:	e8 68 ff ff ff       	call   80490e0 <deregister_tm_clones>
 8049178:	c6 05 24 c0 04 08 01 	movb   $0x1,0x804c024
 804917f:	c9                   	leave  
 8049180:	c3                   	ret    
 8049181:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 8049188:	c3                   	ret    
 8049189:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi

08049190 <frame_dummy>:
 8049190:	f3 0f 1e fb          	endbr32 
 8049194:	eb 8a                	jmp    8049120 <register_tm_clones>

08049196 <malicious_function>:
 8049196:	55                   	push   %ebp
 8049197:	89 e5                	mov    %esp,%ebp
 8049199:	53                   	push   %ebx
 804919a:	83 ec 04             	sub    $0x4,%esp
 804919d:	e8 2e ff ff ff       	call   80490d0 <__x86.get_pc_thunk.bx>
 80491a2:	81 c3 5e 2e 00 00    	add    $0x2e5e,%ebx
 80491a8:	83 ec 0c             	sub    $0xc,%esp
 80491ab:	8d 83 08 e0 ff ff    	lea    -0x1ff8(%ebx),%eax
 80491b1:	50                   	push   %eax
 80491b2:	e8 b9 fe ff ff       	call   8049070 <puts@plt>
 80491b7:	83 c4 10             	add    $0x10,%esp
 80491ba:	83 ec 0c             	sub    $0xc,%esp
 80491bd:	8d 83 2c e0 ff ff    	lea    -0x1fd4(%ebx),%eax
 80491c3:	50                   	push   %eax
 80491c4:	e8 a7 fe ff ff       	call   8049070 <puts@plt>
 80491c9:	83 c4 10             	add    $0x10,%esp
 80491cc:	90                   	nop
 80491cd:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 80491d0:	c9                   	leave  
 80491d1:	c3                   	ret    

080491d2 <vulnerable_function>:
 80491d2:	55                   	push   %ebp
 80491d3:	89 e5                	mov    %esp,%ebp
 80491d5:	53                   	push   %ebx
 80491d6:	83 ec 44             	sub    $0x44,%esp
 80491d9:	e8 f2 fe ff ff       	call   80490d0 <__x86.get_pc_thunk.bx>
 80491de:	81 c3 22 2e 00 00    	add    $0x2e22,%ebx
 80491e4:	83 ec 0c             	sub    $0xc,%esp
 80491e7:	8d 83 4a e0 ff ff    	lea    -0x1fb6(%ebx),%eax
 80491ed:	50                   	push   %eax
 80491ee:	e8 5d fe ff ff       	call   8049050 <printf@plt>
 80491f3:	83 c4 10             	add    $0x10,%esp
 80491f6:	83 ec 0c             	sub    $0xc,%esp
 80491f9:	8d 45 b8             	lea    -0x48(%ebp),%eax
 80491fc:	50                   	push   %eax
 80491fd:	e8 5e fe ff ff       	call   8049060 <gets@plt>
 8049202:	83 c4 10             	add    $0x10,%esp
 8049205:	83 ec 08             	sub    $0x8,%esp
 8049208:	8d 45 b8             	lea    -0x48(%ebp),%eax
 804920b:	50                   	push   %eax
 804920c:	8d 83 62 e0 ff ff    	lea    -0x1f9e(%ebx),%eax
 8049212:	50                   	push   %eax
 8049213:	e8 38 fe ff ff       	call   8049050 <printf@plt>
 8049218:	83 c4 10             	add    $0x10,%esp
 804921b:	90                   	nop
 804921c:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 804921f:	c9                   	leave  
 8049220:	c3                   	ret    

08049221 <main>:
 8049221:	8d 4c 24 04          	lea    0x4(%esp),%ecx
 8049225:	83 e4 f0             	and    $0xfffffff0,%esp
 8049228:	ff 71 fc             	push   -0x4(%ecx)
 804922b:	55                   	push   %ebp
 804922c:	89 e5                	mov    %esp,%ebp
 804922e:	53                   	push   %ebx
 804922f:	51                   	push   %ecx
 8049230:	e8 9b fe ff ff       	call   80490d0 <__x86.get_pc_thunk.bx>
 8049235:	81 c3 cb 2d 00 00    	add    $0x2dcb,%ebx
 804923b:	e8 92 ff ff ff       	call   80491d2 <vulnerable_function>
 8049240:	83 ec 0c             	sub    $0xc,%esp
 8049243:	8d 83 77 e0 ff ff    	lea    -0x1f89(%ebx),%eax
 8049249:	50                   	push   %eax
 804924a:	e8 21 fe ff ff       	call   8049070 <puts@plt>
 804924f:	83 c4 10             	add    $0x10,%esp
 8049252:	b8 00 00 00 00       	mov    $0x0,%eax
 8049257:	8d 65 f8             	lea    -0x8(%ebp),%esp
 804925a:	59                   	pop    %ecx
 804925b:	5b                   	pop    %ebx
 804925c:	5d                   	pop    %ebp
 804925d:	8d 61 fc             	lea    -0x4(%ecx),%esp
 8049260:	c3                   	ret    

Disassembly of section .fini:

08049264 <_fini>:
 8049264:	f3 0f 1e fb          	endbr32 
 8049268:	53                   	push   %ebx
 8049269:	83 ec 08             	sub    $0x8,%esp
 804926c:	e8 5f fe ff ff       	call   80490d0 <__x86.get_pc_thunk.bx>
 8049271:	81 c3 8f 2d 00 00    	add    $0x2d8f,%ebx
 8049277:	83 c4 08             	add    $0x8,%esp
 804927a:	5b                   	pop    %ebx
 804927b:	c3                   	ret    
