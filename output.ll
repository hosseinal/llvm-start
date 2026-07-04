; ModuleID = 'Hllvm'
source_filename = "Hllvm"

@x = constant i32 42, align 4
@xx = constant i32 48, align 4
@0 = private unnamed_addr constant [8 x i8] c"X: %d\\n\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @0, i32 0, i32 0), i32 48)
  ret i32 0
}
