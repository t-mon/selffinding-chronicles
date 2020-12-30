extends AnimationPlayer

# Settings
var animationBaseDirectory =  ProjectSettings.globalize_path("res://") + "../animations/"
var finalSize = Vector2(200, 200)
var resolution = 2

# Objects
var defaultViewport
var timer
var currentSpriteImageRight
var currentSpriteImageLeft

# Runtime variables
var frame = 1
var seekStepSize = 1.0 / resolution
var currentAnimationName
var currentAnimationDirectory
var animations = get_animation_list()

func _process(delta):
	pass

func _ready():
	defaultViewport = get_viewport()
	defaultViewport.set_transparent_background(true)
	print("Viewport bg transp: ", defaultViewport.transparent_bg)
	print("Viewport size: ", defaultViewport.size)
	print("Save renderings into path ", animationBaseDirectory)
	
	# Set to manual so we can seek manually
	set_animation_process_mode(AnimationPlayer.ANIMATION_PROCESS_MANUAL)
	set_method_call_mode(AnimationPlayer.ANIMATION_METHOD_CALL_IMMEDIATE)
	
	# Inform about start and end
	#connect("animation_started", self, "_on_AnimationPlayer_animation_started")
	#connect("animation_finished", self, "_on_AnimationPlayer_animation_finished")
	print("Available animations ", get_animation_list())
	
	# Timer for rendering images
	timer = Timer.new()
	add_child(timer) 
	timer.connect("timeout",self,"_on_timer_timeout") 
	print("Ready. Start rendering animations...")
	renderNextAnimation()


func renderNextAnimation():
	if animations.empty():
		print("Finished. All animations rendered")
		return
	
	# Take the next animation
	currentAnimationName = animations[0]
	animations.remove(0)
	currentAnimationDirectory = animationBaseDirectory + currentAnimationName
	print("Start rendering animation \"", currentAnimationName, "\" Animations left: ", animations)
	var directory  = Directory.new()
	if !directory.dir_exists(currentAnimationDirectory):
		print("Create animation directory ", currentAnimationDirectory)
		directory.make_dir_recursive(currentAnimationDirectory)
	directory.open(currentAnimationDirectory)
	
	# Begin with the animation
	frame = 1
	set_current_animation(currentAnimationName) 
	stop(true)
	seek(0, true)
	set_current_animation(currentAnimationName) 
	
	# Create the current sprite image
	currentSpriteImageRight = Image.new()
	currentSpriteImageRight.create(current_animation_length * resolution * finalSize.x, finalSize.y, false, Image.FORMAT_RGBA8)
	currentSpriteImageLeft = Image.new()
	currentSpriteImageLeft.create(current_animation_length * resolution * finalSize.x, finalSize.y, false, Image.FORMAT_RGBA8)
	
	timer.start(1)


func saveImage(image, name, index):
	image.convert(Image.FORMAT_RGBA8)
	image.flip_y()
	image.resize(finalSize.x, finalSize.y, Image.INTERPOLATE_LANCZOS)
	var finalName = currentAnimationDirectory + "/" + name + "-right-%02d.png" % index
	print("Saving image ", finalName)
	image.save_png(finalName)
	
	# Write each pixel into the big sprite image (very slow, but not important here)
	var width = finalSize.x
	var height = finalSize.y
	for x in range(0, width - 1):
		for y in range(0, height - 1):
			image.lock()
			var pixel = image.get_pixel(x, y);
			image.unlock()
			currentSpriteImageRight.lock()
			currentSpriteImageRight.set_pixel((index - 1) * width + x, y, pixel)
			currentSpriteImageRight.unlock()

	# Render mirrored
	image.flip_x()
	finalName = currentAnimationDirectory + "/" + name + "-left-%02d.png" % index
	print("Saving image ", finalName)
	image.save_png(finalName)
	for x in range(0, width - 1):
		for y in range(0, height - 1):
			image.lock()
			var pixel = image.get_pixel(x, y);
			image.unlock()
			currentSpriteImageLeft.lock()
			currentSpriteImageLeft.set_pixel((index - 1) * width + x, y, pixel)
			currentSpriteImageLeft.unlock()


func _on_timer_timeout():
	seek(frame * seekStepSize, true)
	print("-->  ", frame, " ", current_animation_position, "/", current_animation_length)
	var image = defaultViewport.get_texture().get_data()
	saveImage(image, currentAnimationName, frame)
	frame += 1
	if (frame >= (current_animation_length * resolution) + 1):
		timer.stop()
		currentSpriteImageRight.save_png(currentAnimationDirectory + "/" + currentAnimationName + "-right.png")
		currentSpriteImageLeft.save_png(currentAnimationDirectory + "/" + currentAnimationName + "-left.png")
		print("Done rendering ", currentAnimationName)
		renderNextAnimation()


#func _on_AnimationPlayer_animation_started(animationName):
#	print("Animation started ", animationName)


#func _on_AnimationPlayer_animation_finished(animationName):
#	print("Animation finished ", animationName)

